// Copyright 2023 The Chromium Authors
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

/**
 * @fileoverview xf-cloud-panel element.
 * Disable type checking for closure, as it is done by the typescript compiler.
 * @suppress {checkTypes}
 */

import {str, strf, util} from '../common/js/util.js';
import {constants} from '../foreground/js/constants.js';

import {css, customElement, html, property, query, XfBase} from './xf_base.js';

import type {CrActionMenuElement} from 'chrome://resources/cr_elements/cr_action_menu/cr_action_menu.js';

/**
 * These type indicate static states that the cloud panel can enter. If one of
 * these is supplied, `items` and `percentage` is ignored.
 */
export enum CloudPanelType {
  OFFLINE = 'offline',
  NOT_ENOUGH_SPACE = 'not-enough-space',
}

/**
 * The `<xf-cloud-panel>` represents the current state that the Drive bulk
 * pinning process is currently in. When files are being pinned and downloaded,
 * the `items` and `progress` attributes are used to signify that the panel is
 * in progress. The `type` attribute can be used with `not-enough-space` and
 * `offline` to signify possible error or paused states.
 */
@customElement('xf-cloud-panel')
export class XfCloudPanel extends XfBase {
  /**
   * The number of items currently syncing.
   */
  @property({type: Number, reflect: true, attribute: true}) items?: number;

  /**
   * The percentage that should be represented in the progress bar, this also
   * ensures the value is a valid value within the range [0, 100].
   */
  @property({
    type: Number,
    reflect: true,
    converter: {
      fromAttribute:
          (value: string) => {
            let percentage = null;
            try {
              percentage = parseInt(value, 10);
            } catch (e) {
              return null;
            }
            if (util.isNullOrUndefined(percentage) ||
                Number.isNaN(percentage) || percentage < 0 ||
                percentage > 100) {
              return null;
            }
            return percentage;
          },
      toAttribute: (value: number) => String(value),
    },
  })
  percentage?: number;

  /**
   * Attempts to map the supplied `type` attribute to an available value.
   */
  @property({
    type: CloudPanelType,
    reflect: true,
    converter: {
      fromAttribute:
          (value: string) => {
            if (value in CloudPanelType) {
              return value as CloudPanelType;
            }
            console.warn(`Failed to convert ${value} to CloudPanelType`);
            return null;
          },
      toAttribute: (key: keyof CloudPanelType) => key,
    },
  })
  type?: CloudPanelType;

  /**
   * The cloud panel uses the `CrActionMenu` to provide the dialog behaviour and
   * the overlay logic.
   */
  @query('cr-action-menu') private $panel_?: CrActionMenuElement;

  /**
   * Provide a number formatter that matches the users locale.
   */
  private numberFormatter_ =
      new Intl.NumberFormat(util.getCurrentLocaleOrDefault());

  static get events() {
    return {
      DRIVE_SETTINGS_CLICKED: 'drive_settings_clicked',
    } as const;
  }

  static override get styles() {
    return getCSS();
  }

  /**
   * Returns true if the dialog is open, false otherwise.
   */
  get open() {
    return this.$panel_?.open || false;
  }

  /**
   * Show the element relative to the cloud icon that was clicked.
   */
  showAt(el: HTMLElement) {
    this.$panel_!.showAt(el, {top: el.offsetTop + el.offsetHeight + 8});
  }

  /**
   * Close the panel.
   */
  close() {
    if (this.open) {
      this.$panel_!.close();
    }
  }

  /**
   * Handles click events for the Google Drive settings button. This emits the
   * event to be handled by the container.
   */
  private onSettingsClicked_(event: MouseEvent|KeyboardEvent) {
    event.stopImmediatePropagation();
    event.preventDefault();

    if ((event as KeyboardEvent).repeat) {
      return;
    }

    this.dispatchEvent(
        new CustomEvent(XfCloudPanel.events.DRIVE_SETTINGS_CLICKED, {
          bubbles: true,
          composed: true,
        }));
  }

  override render() {
    return html`<cr-action-menu>
      <div class="body">
        <div id="progress-state">
          <div class="progress">${
        this.items && this.items > 1 ?
            strf(
                'DRIVE_MULTIPLE_FILES_SYNCING',
                this.numberFormatter_.format(this.items)) :
            str('DRIVE_SINGLE_FILE_SYNCING')}</div>
          <progress
              class="progress-bar"
              max="100"
              value="${this.percentage}">
            ${this.percentage}%
          </progress>
          <div class="progress-description">3 minutes remaining</div>
        </div>
        <div class="static" id="progress-finished">
          <xf-icon type="${
        constants.ICON_TYPES.BULK_PINNING_DONE}" size="large"></xf-icon>
          <div class="status-description">
            ${str('DRIVE_ALL_FILES_SYNCED')}
          </div>
        </div>
        <div class="static" id="progress-offline">
        <xf-icon type="${
        constants.ICON_TYPES.BULK_PINNING_OFFLINE}" size="large"></xf-icon>
          <div class="status-description">
            ${str('DRIVE_BULK_PINNING_OFFLINE')}
          </div>
        </div>
        <div class="static" id="progress-not-enough-space">
        <xf-icon type="${
        constants.ICON_TYPES.ERROR_BANNER}" size="large"></xf-icon>
          <div class="status-description">
            ${str('DRIVE_BULK_PINNING_NOT_ENOUGH_SPACE')}
          </div>
        </div>
        <div class="divider"></div>
        <div class="menu">
          <button class="action" @click=${this.onSettingsClicked_}>${
        str('GOOGLE_DRIVE_SETTINGS_LINK')}</div>
        </div>
      </div>
    </cr-action-menu>`;
  }
}

function getCSS() {
  return css`
    :host {
      position: absolute;
      right: 0px;
      top: 50px;
      z-index: 600;
    }

    :host(:not([items][percentage])) #progress-state,
    :host([percentage="100"]) #progress-state,
    :host([type]) #progress-state {
      display: none;
    }

    :host(:not([items][percentage="100"])) #progress-finished,
    :host([type]) #progress-finished {
      display: none;
    }

    :host(:not([type="offline"])) #progress-offline {
      display: none;
    }

    :host(:not([type="not-enough-space"])) #progress-not-enough-space {
      display: none;
    }

    .body {
      display: flex;
      flex-direction: column;
      margin: -8px 0;
      width: 320px;
    }

    .static {
      align-items: center;
      display: flex;
      flex-direction: column;
    }

    xf-icon {
      padding: 27px 0px 20px;
    }

    xf-icon[type="bulk_pinning_done"] {
      --xf-icon-color: var(--cros-sys-positive);
    }

    xf-icon[type="bulk_pinning_offline"] {
      --xf-icon-color: var(--cros-sys-secondary);
    }

    xf-icon[type="error_banner"] {
      --xf-icon-color: var(--cros-sys-error);
    }

    .status-description {
      color: var(--cros-text-color-secondary);
      font-size: 13px;
      line-height: 20px;
      padding: 0px 16px 20px;
      text-align: center;
    }

    .progress {
      color: var(--cros-text-color-primary);
      font-size: 13px;
      font-weight: 500;
      line-height: 20px;
      margin-inline: 16px;
      padding-top: 20px;
    }

    .progress-description {
      color: var(--cros-text-color-secondary);
      padding-bottom: 20px;
      padding-inline: 16px;
    }

    .progress-bar {
      background-color: var(--cros-sys-primary_container);
      border-radius: 10px;
      height: 4px;
      margin: 8px 0 8px;
      margin-inline: 16px;
      width: calc(100% - 32px);
    }

    progress::-webkit-progress-bar {
      border-radius: 10px;
    }

    progress.progress-bar::-webkit-progress-value {
      background-color: var(--cros-sys-primary);
      border-radius: 10px;
    }

    .divider {
      background: var(--cros-sys-separator);
      height: 1px;
      width: 100%;
    }

    button.action {
      background-color: var(--cros-sys-base_elevated);
      border: 0;
      text-align: left;
    }

    .action {
      font-size: 13px;
      font-weight: 500;
      line-height: 8px;
      padding: 20px 0 20px;
      padding-inline: 16px;
      width: 100%;
    }

    .action:hover {
      background: var(--cros-sys-hover_on_subtle);
    }

    .menu {
      height: 48px;
    }
  `;
}

export type CloudPanelSettingsClickEvent = CustomEvent;

declare global {
  interface HTMLElementEventMap {
    [XfCloudPanel.events.DRIVE_SETTINGS_CLICKED]: CloudPanelSettingsClickEvent;
  }

  interface HTMLElementTagNameMap {
    'xf-cloud-panel': XfCloudPanel;
  }
}
