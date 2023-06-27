// Copyright 2021 The Chromium Authors
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

import './input_key.js';
import 'chrome://resources/cr_elements/cr_input/cr_input.js';

import {I18nMixin} from 'chrome://resources/cr_elements/i18n_mixin.js';
import {assert, assertNotReached} from 'chrome://resources/js/assert_ts.js';
import {String16} from 'chrome://resources/mojo/mojo/public/mojom/base/string16.mojom-webui.js';
import {PolymerElementProperties} from 'chrome://resources/polymer/v3_0/polymer/interfaces.js';
import {PolymerElement} from 'chrome://resources/polymer/v3_0/polymer/polymer_bundled.min.js';

import {AcceleratorResultData} from '../mojom-webui/ash/webui/shortcut_customization_ui/mojom/shortcut_customization.mojom-webui.js';

import {AcceleratorLookupManager} from './accelerator_lookup_manager.js';
import {getTemplate} from './accelerator_view.html.js';
import {keyToIconNameMap} from './input_key.js';
import {getShortcutProvider} from './mojo_interface_provider.js';
import {mojoString16ToString} from './mojo_utils.js';
import {ModifierKeyCodes} from './shortcut_input.js';
import {Accelerator, AcceleratorConfigResult, AcceleratorSource, Modifier, ShortcutProviderInterface, StandardAcceleratorInfo} from './shortcut_types.js';
import {createEmptyAcceleratorInfo, getAccelerator, getModifiersForAcceleratorInfo, isCustomizationDisabled, isFunctionKey} from './shortcut_utils.js';

export interface AcceleratorViewElement {
  $: {
    container: HTMLDivElement,
  };
}

enum KeyState {
  NOT_SELECTED = 'not-selected',
  MODIFIER = 'modifier-selected',
  ALPHANUMERIC = 'alpha-numeric-selected',
}

export enum ViewState {
  VIEW,
  ADD,
  EDIT,
}

/**
 * @fileoverview
 * 'accelerator-view' is wrapper component for an accelerator. It maintains both
 * the read-only and editable state of an accelerator.
 * TODO(jimmyxgong): Implement the edit mode.
 */
const AcceleratorViewElementBase = I18nMixin(PolymerElement);

export class AcceleratorViewElement extends AcceleratorViewElementBase {
  static get is(): string {
    return 'accelerator-view';
  }

  static get properties(): PolymerElementProperties {
    return {
      acceleratorInfo: {
        type: Object,
      },

      pendingAcceleratorInfo: {
        type: Object,
      },

      viewState: {
        type: Number,
        value: ViewState.VIEW,
        notify: true,
        observer: AcceleratorViewElement.prototype.onViewStateChanged,
      },

      modifiers: {
        type: Array,
        computed: 'getModifiers(acceleratorInfo.accelerator.*)',
      },

      isCapturing: {
        type: Boolean,
        value: false,
      },

      statusMessage: {
        type: String,
        notify: true,
      },

      /** Informs parent components that an error has occurred. */
      hasError: {
        type: Boolean,
        value: false,
        notify: true,
      },

      action: {
        type: Number,
        value: 0,
      },

      source: {
        type: Number,
        value: 0,
      },

      sourceIsLocked: {
        type: Boolean,
        value: false,
      },

      /**
       * Conditionally show the edit-icon-container in `accelerator-view`, true
       * for `accelerator-row`, false for `accelerator-edit-view`.
       */
      showEditIcon: {
        type: Boolean,
        value: false,
      },

      /** Only show the edit button in the first row. */
      isFirstAccelerator: {
        type: Boolean,
      },
    };
  }

  acceleratorInfo: StandardAcceleratorInfo;
  viewState: ViewState;
  statusMessage: string;
  hasError: boolean;
  action: number;
  source: AcceleratorSource;
  sourceIsLocked: boolean;
  showEditIcon: boolean;
  categoryIsLocked: boolean;
  isFirstAccelerator: boolean;
  protected pendingAcceleratorInfo: StandardAcceleratorInfo;
  private modifiers: string[];
  private isCapturing: boolean;
  private shortcutProvider: ShortcutProviderInterface = getShortcutProvider();
  private lookupManager: AcceleratorLookupManager =
      AcceleratorLookupManager.getInstance();

  override connectedCallback(): void {
    super.connectedCallback();

    this.categoryIsLocked = this.lookupManager.isCategoryLocked(
        this.lookupManager.getAcceleratorCategory(this.source, this.action));
  }

  private getModifiers(): string[] {
    return getModifiersForAcceleratorInfo(this.acceleratorInfo);
  }

  protected onViewStateChanged(): void {
    if (this.viewState !== ViewState.VIEW) {
      this.registerKeyEventListeners();
      return;
    }
    this.unregisterKeyEventListeners();
  }

  private registerKeyEventListeners(): void {
    this.addEventListener('keydown', (e) => this.onKeyDown(e));
    this.addEventListener('keyup', (e) => this.onKeyUp(e));
    this.addEventListener('focus', () => this.startCapture());
    this.addEventListener('mouseup', () => this.startCapture());
    this.addEventListener('blur', () => this.endCapture());
    this.$.container.focus();
  }

  private unregisterKeyEventListeners(): void {
    this.removeEventListener('keydown', (e) => this.onKeyDown(e));
    this.removeEventListener('keyup', (e) => this.onKeyUp(e));
    this.removeEventListener('focus', () => this.startCapture());
    this.removeEventListener('mouseup', () => this.startCapture());
    this.removeEventListener('blur', () => this.endCapture());
  }


  private async startCapture(): Promise<void> {
    if (this.isCapturing) {
      return;
    }
    // Disable ChromeOS accelerator handler when starting input capture.
    this.pendingAcceleratorInfo = createEmptyAcceleratorInfo();
    this.isCapturing = true;

    this.dispatchEvent(new CustomEvent('accelerator-capturing-started', {
      bubbles: true,
      composed: true,
    }));

    // Block processing accelerators.
    await this.shortcutProvider.preventProcessingAccelerators(true);
  }

  private async endCapture(): Promise<void> {
    if (!this.isCapturing) {
      return;
    }

    this.viewState = ViewState.VIEW;
    this.statusMessage = '';
    this.hasError = false;
    this.isCapturing = false;
    this.pendingAcceleratorInfo = createEmptyAcceleratorInfo();

    this.dispatchEvent(new CustomEvent('accelerator-capturing-ended', {
      bubbles: true,
      composed: true,
    }));

    await this.shortcutProvider.preventProcessingAccelerators(false);
  }

  private onKeyDown(e: KeyboardEvent): void {
    this.handleKey(e);
  }

  private onKeyUp(e: KeyboardEvent): void {
    e.preventDefault();
    e.stopPropagation();
    // TODO(jimmyxgong): Check for errors e.g. accelerator conflicts.
  }

  private handleKey(e: KeyboardEvent): void {
    // While capturing, we prevent all events from bubbling, to prevent
    // shortcuts from executing and interrupting the input capture.
    e.preventDefault();
    e.stopPropagation();

    // Add the key pressed to pendingAccelerator.
    this.set(
        'pendingAcceleratorInfo.layoutProperties.standardAccelerator.accelerator',
        this.keystrokeToAccelerator(e));

    if (this.isModifierKey(e)) {
      // Reset the keyDisplay property if the key is a modifier.
      this.set(
          'pendingAcceleratorInfo.layoutProperties.standardAccelerator.keyDisplay',
          '');
    } else {
      // Set keyDisplay property.
      this.set(
          'pendingAcceleratorInfo.layoutProperties.standardAccelerator.keyDisplay',
          this.getKeyDisplay(e));
    }

    // Only process valid accelerators.
    if (this.isValidDefaultAccelerator(this.pendingAcceleratorInfo)) {
      this.processPendingAccelerator(this.pendingAcceleratorInfo);
    }
  }

  private async processPendingAccelerator(
      pendingAccelInfo: StandardAcceleratorInfo): Promise<void> {
    // Reset status state when processing the new accelerator.
    this.statusMessage = '';
    this.hasError = false;

    let result: {result: AcceleratorResultData};
    assert(this.viewState !== ViewState.VIEW);

    if (this.viewState === ViewState.ADD) {
      result = await this.shortcutProvider.addAccelerator(
          this.source, this.action, getAccelerator(pendingAccelInfo));
    }

    if (this.viewState === ViewState.EDIT) {
      result = await this.shortcutProvider.replaceAccelerator(
          this.source, this.action, getAccelerator(this.acceleratorInfo),
          getAccelerator(pendingAccelInfo));
    }
    this.handleAcceleratorResultData(result!.result);
  }

  // TODO(longbowei): Finalize and localize these messages.
  private handleAcceleratorResultData(result: AcceleratorResultData): void {
    switch (result.result) {
      // Shift is the only modifier.
      case AcceleratorConfigResult.kShiftOnlyNotAllowed: {
        this.statusMessage =
            'Shortcut is not valid. Shift can not be used as the only ' +
            'modifier key. Press a new shortcut.';
        this.hasError = true;
        return;
      }
      // No modifiers is pressed before primary key.
      case AcceleratorConfigResult.kMissingModifier: {
        // This is a backup check, since only valid accelerators are processed
        // and a valid accelerator will have modifier(s) and a key or is
        // function key.
        this.statusMessage =
            'Shortcut is not valid. Must include at lease one modifier key. ' +
            'Press a new shortcut.';
        this.hasError = true;
        return;
      }
      // Top row key used as activation keys(no search key pressed).
      case AcceleratorConfigResult.kKeyNotAllowed: {
        this.statusMessage =
            'Shortcut with top row keys need to include the search key.';
        this.hasError = true;
        return;
      }
      // Conflict with a locked accelerator.
      case AcceleratorConfigResult.kConflict:
      case AcceleratorConfigResult.kActionLocked: {
        this.statusMessage = this.i18n(
            'lockedShortcutStatusMessage',
            mojoString16ToString(result.shortcutName as String16));
        this.hasError = true;
        return;
      }
      // Conflict with an editable shortcut.
      case AcceleratorConfigResult.kConflictCanOverride: {
        this.statusMessage = this.i18n(
            'shortcutWithConflictStatusMessage',
            mojoString16ToString(result.shortcutName as String16));
        this.hasError = true;
        return;
      }
      // Limit to only 5 accelerators allowed.
      case AcceleratorConfigResult.kMaximumAcceleratorsReached: {
        this.statusMessage = 'Maximum accelerators have reached.';
        this.hasError = true;
        return;
      }
      case AcceleratorConfigResult.kSuccess: {
        this.pendingAcceleratorInfo = createEmptyAcceleratorInfo();
        this.fireUpdateEvent();
        return;
      }
    }
    assertNotReached();
  }

  /**
   * Converts a keystroke event to an Accelerator Object.
   */
  private keystrokeToAccelerator(e: KeyboardEvent): Accelerator {
    const output: Accelerator = {modifiers: 0, keyCode: 0};
    if (e.metaKey) {
      output.modifiers = output.modifiers | Modifier.COMMAND;
    }
    if (e.ctrlKey) {
      output.modifiers = output.modifiers | Modifier.CONTROL;
    }
    if (e.altKey) {
      output.modifiers = output.modifiers | Modifier.ALT;
    }
    // Shift key isn't registered as a modifier unless a non-modifer key is
    // pressed in conjunction with the keystroke.
    if (e.key == 'Shift' || e.shiftKey) {
      output.modifiers = output.modifiers | Modifier.SHIFT;
    }

    // Only add non-modifier or function keys as the pending key.
    if (!this.isModifierKey(e) || isFunctionKey(e.keyCode)) {
      output.keyCode = e.keyCode;
    }

    return output;
  }

  private getKeyDisplay(e: KeyboardEvent): string {
    switch (e.code) {
      case 'Space':  // Space key: e.key: ' ', e.code: 'Space', set keyDisplay
                     // to be 'space' text.
        return 'space';
      case 'ShowAllWindows':  // Overview key: e.key: 'F4', e.code:
                              // 'ShowAllWindows', set keyDisplay to be
                              // 'LaunchApplication1' and will display as
                              // 'overview' icon.
        return 'LaunchApplication1';
      default:  // All other keys: Use the original e.key as keyDisplay.
        return e.key;
    }
  }

  private isModifierKey(e: KeyboardEvent): boolean {
    return ModifierKeyCodes.includes(e.keyCode);
  }

  /**
   * Returns the specified CSS state of the modifier key element.
   */
  protected getCtrlState(): string {
    return this.getModifierState(Modifier.CONTROL);
  }

  /**
   * Returns the specified CSS state of the modifier key element.
   */
  protected getAltState(): string {
    return this.getModifierState(Modifier.ALT);
  }

  /**
   * Returns the specified CSS state of the modifier key element.
   */
  protected getShiftState(): string {
    return this.getModifierState(Modifier.SHIFT);
  }

  /**
   * Returns the specified CSS state of the modifier key element.
   */
  protected getSearchState(): string {
    return this.getModifierState(Modifier.COMMAND);
  }

  /**
   * Returns the specified CSS state of the modifier key element.
   */
  private getModifierState(modifier: Modifier): KeyState {
    if ((getAccelerator(this.pendingAcceleratorInfo)).modifiers & modifier) {
      return KeyState.MODIFIER;
    }
    return KeyState.NOT_SELECTED;
  }

  /**
   * Returns the specified CSS state of the pending key element.
   */
  protected getPendingKeyState(): string {
    if (this.pendingAcceleratorInfo.layoutProperties.standardAccelerator
            .keyDisplay != '') {
      return KeyState.ALPHANUMERIC;
    }
    return KeyState.NOT_SELECTED;
  }

  /**
   * Returns the specified key to display.
   */
  protected getPendingKey(): string {
    if (this.pendingAcceleratorInfo.layoutProperties.standardAccelerator
            .keyDisplay != '') {
      const keyDisplay = this.pendingAcceleratorInfo.layoutProperties
                             .standardAccelerator.keyDisplay;
      // Display as icon if it exists in the map.
      if (keyDisplay in keyToIconNameMap) {
        return keyDisplay;
      }
      return keyDisplay.toLowerCase();
    }
    // TODO(jimmyxgong): Reset to a localized default empty state.
    return 'key';
  }

  /**
   * Returns true if the event has valid modifiers.
   */
  private hasValidModifiers(e: KeyboardEvent): boolean {
    // Although Shift is a modifier, it cannot be a standalone modifier for a
    // shortcut.
    return e.ctrlKey || e.altKey || e.metaKey;
  }

  private isValidDefaultAccelerator(accelInfo: StandardAcceleratorInfo):
      boolean {
    // A valid default accelerator is one that has modifier(s) and a key or
    // is function key.
    const accelerator =
        accelInfo.layoutProperties.standardAccelerator.accelerator;
    return (accelerator.modifiers > 0 &&
            accelInfo.layoutProperties.standardAccelerator.keyDisplay !== '') ||
        isFunctionKey(accelerator.keyCode);
  }

  private showEditView(): boolean {
    return this.viewState !== ViewState.VIEW;
  }

  private fireUpdateEvent(): void {
    this.dispatchEvent(new CustomEvent('request-update-accelerator', {
      bubbles: true,
      composed: true,
      detail: {source: this.source, action: this.action},
    }));

    // Always end input capturing if an update event was fired.
    this.endCapture();
  }

  private shouldShowLockIcon(): boolean {
    // Do not show lock icon in each row if customization is disabled or its
    // category is locked.
    if (isCustomizationDisabled() || this.categoryIsLocked) {
      return false;
    }
    // Show lock icon if accelerator is locked.
    return (this.acceleratorInfo && this.acceleratorInfo.locked) ||
        this.sourceIsLocked;
  }

  private shouldShowEditIcon(): boolean {
    // Do not show edit icon in each row if customization is disabled, the row
    // is displayed in edit-dialog(!showEditIcon) or category is locked.
    if (isCustomizationDisabled() || !this.showEditIcon ||
        this.categoryIsLocked) {
      return false;
    }
    // Show edit icon if accelerator is not locked.
    return !(this.acceleratorInfo && this.acceleratorInfo.locked) &&
        !this.sourceIsLocked && this.isFirstAccelerator;
  }

  private onEditIconClicked(): void {
    this.dispatchEvent(
        new CustomEvent('edit-icon-clicked', {bubbles: true, composed: true}));
  }

  static get template(): HTMLTemplateElement {
    return getTemplate();
  }
}

declare global {
  interface HTMLElementTagNameMap {
    'accelerator-view': AcceleratorViewElement;
  }
}

customElements.define(AcceleratorViewElement.is, AcceleratorViewElement);
