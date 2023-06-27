// Copyright 2019 The Chromium Authors
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

import {assert, assertNotReached} from '../assert.js';
import {reportError} from '../error.js';
import {Point} from '../geometry.js';
import {
  ErrorLevel,
  ErrorType,
  MimeType,
} from '../type.js';
import {windowController} from '../window_controller.js';

import {
  CameraAppHelper,
  CameraAppHelperRemote,
  CameraIntentAction,
  DocumentOutputFormat,
  DocumentScannerReadyState,
  ExternalScreenMonitorCallbackRouter,
  FileMonitorResult,
  Rotation,
  ScreenState,
  ScreenStateMonitorCallbackRouter,
  StorageMonitorCallbackRouter,
  StorageMonitorStatus,
  TabletModeMonitorCallbackRouter,
  ToteMetricFormat,
} from './type.js';
import {wrapEndpoint} from './util.js';

/**
 * The singleton instance of ChromeHelper. Initialized by the first
 * invocation of getInstance().
 */
let instance: ChromeHelper|null = null;

/**
 * Forces casting type from Uint8Array to number[].
 */
function castToNumberArray(data: Uint8Array): number[] {
  // This cast is to workaround that the generated mojo binding only accepts
  // number[], but actually can be passed Uint8Array (which also supports
  // indexing via [] and length).
  // eslint-disable-next-line @typescript-eslint/consistent-type-assertions
  return data as unknown as number[];
}

/**
 * Casts from rotation degrees to mojo rotation.
 */
function castToMojoRotation(rotation: number): Rotation {
  switch (rotation) {
    case 0:
      return Rotation.ROTATION_0;
    case 90:
      return Rotation.ROTATION_90;
    case 180:
      return Rotation.ROTATION_180;
    case 270:
      return Rotation.ROTATION_270;
    default:
      assertNotReached(`Invalid rotation ${rotation}`);
  }
}

/**
 * Communicates with Chrome.
 */
export class ChromeHelper {
  /**
   * An interface remote that is used to communicate with Chrome.
   */
  private readonly remote: CameraAppHelperRemote =
      wrapEndpoint(CameraAppHelper.getRemote());

  /**
   * Starts monitoring tablet mode state of device.
   *
   * @param onChange Callback called each time when tablet mode state of device
   *     changes with boolean parameter indicating whether device is entering
   *     tablet mode.
   * @return Resolved to initial tablet mode state of device.
   */
  async initTabletModeMonitor(onChange: (isTablet: boolean) => void):
      Promise<boolean> {
    const monitorCallbackRouter =
        wrapEndpoint(new TabletModeMonitorCallbackRouter());
    monitorCallbackRouter.update.addListener(onChange);

    const {isTabletMode} = await this.remote.setTabletMonitor(
        monitorCallbackRouter.$.bindNewPipeAndPassRemote());
    return isTabletMode;
  }

  /**
   * Starts monitoring screen state of device.
   *
   * @param onChange Callback called each time when screen state of device
   *     changes with parameter of newly changed value.
   * @return Resolved to initial screen state of device.
   */
  async initScreenStateMonitor(onChange: (state: ScreenState) => void):
      Promise<ScreenState> {
    const monitorCallbackRouter =
        wrapEndpoint(new ScreenStateMonitorCallbackRouter());
    monitorCallbackRouter.update.addListener(onChange);

    const {initialState} = await this.remote.setScreenStateMonitor(
        monitorCallbackRouter.$.bindNewPipeAndPassRemote());
    return initialState;
  }

  /**
   * Starts monitoring the existence of external screens.
   *
   * @param onChange Callback called each time when the existence of external
   *     screens changes.
   * @return Resolved to the initial state of external screens existence.
   */
  async initExternalScreenMonitor(
      onChange: (hasExternalScreen: boolean) => void): Promise<boolean> {
    const monitorCallbackRouter =
        wrapEndpoint(new ExternalScreenMonitorCallbackRouter());
    monitorCallbackRouter.update.addListener(onChange);

    const {hasExternalScreen} = await this.remote.setExternalScreenMonitor(
        monitorCallbackRouter.$.bindNewPipeAndPassRemote());
    return hasExternalScreen;
  }

  async isTabletMode(): Promise<boolean> {
    const {isTabletMode} = await this.remote.isTabletMode();
    return isTabletMode;
  }

  /**
   * Initializes the camera window controller and bootstraps the mojo
   * communication to get window states.
   */
  async initCameraWindowController(): Promise<void> {
    let {controller} = await this.remote.getWindowStateController();
    controller = wrapEndpoint(controller);
    await windowController.bind(controller);
  }

  /**
   * Starts event tracing of |event| in Chrome.
   */
  startTracing(event: string): void {
    this.remote.startPerfEventTrace(event);
  }

  /**
   * Stops event tracing of |event| in Chrome.
   */
  stopTracing(event: string): void {
    this.remote.stopPerfEventTrace(event);
  }

  /**
   * Opens the file in Downloads folder by its |name| in gallery.
   */
  openFileInGallery(name: string): void {
    this.remote.openFileInGallery(name);
  }

  /**
   * Opens the chrome feedback dialog.
   *
   * @param placeholder The text of the placeholder in the description
   *     field.
   */
  openFeedbackDialog(placeholder: string): void {
    this.remote.openFeedbackDialog(placeholder);
  }

  /**
   * Opens the given |url| in the browser.
   */
  openUrlInBrowser(url: string): void {
    this.remote.openUrlInBrowser({url: url});
  }

  /**
   * Checks |value| returned from handleCameraResult() succeed.
   */
  private async checkReturn(
      caller: string, value: Promise<{isSuccess: boolean}>): Promise<void> {
    const {isSuccess} = await value;
    if (!isSuccess) {
      reportError(
          ErrorType.HANDLE_CAMERA_RESULT_FAILURE, ErrorLevel.ERROR,
          new Error(`Return not isSuccess from calling intent ${caller}.`));
    }
  }

  /**
   * Notifies ARC++ to finish the intent of given |intendId|.
   */
  async finish(intentId: number): Promise<void> {
    const ret =
        this.remote.handleCameraResult(intentId, CameraIntentAction.FINISH, []);
    await this.checkReturn('finish()', ret);
  }

  /**
   * Notifies ARC++ to append |data| to intent of the given |intentId|.
   *
   * @param intentId Intent id of the intent which |data| appends to.
   * @param data The data to be appended to intent result.
   */
  async appendData(intentId: number, data: Uint8Array): Promise<void> {
    const ret = this.remote.handleCameraResult(
        intentId, CameraIntentAction.APPEND_DATA, castToNumberArray(data));
    await this.checkReturn('appendData()', ret);
  }

  /**
   * Notifies ARC++ to clear appended intent result data.
   *
   * @param intentId Intent id of the intent whose results to be cleared.
   */
  async clearData(intentId: number): Promise<void> {
    const ret = this.remote.handleCameraResult(
        intentId, CameraIntentAction.CLEAR_DATA, []);
    await this.checkReturn('clearData()', ret);
  }

  /**
   * Returns if the logging consent option is enabled.
   */
  async isMetricsAndCrashReportingEnabled(): Promise<boolean> {
    const {isEnabled} = await this.remote.isMetricsAndCrashReportingEnabled();
    return isEnabled;
  }

  /**
   * Sends the broadcast to ARC to notify the new photo/video is captured.
   */
  sendNewCaptureBroadcast({isVideo, name}: {isVideo: boolean, name: string}):
      void {
    this.remote.sendNewCaptureBroadcast(isVideo, name);
  }

  /**
   * Notifies Tote client when a photo/pdf/video/gif is captured.
   */
  notifyTote(format: ToteMetricFormat, name: string): void {
    this.remote.notifyTote(format, name);
  }

  /**
   * Monitors for the file deletion of the file given by its |name| and triggers
   * |callback| when the file is deleted. Note that a previous monitor request
   * will be canceled once another monitor request is sent.
   *
   * @return Resolved when the file is deleted or the current monitor is
   *     canceled by future monitor call.
   * @throws When error occurs during monitor.
   */
  async monitorFileDeletion(name: string, callback: () => void): Promise<void> {
    const {result} = await this.remote.monitorFileDeletion(name);
    switch (result) {
      case FileMonitorResult.DELETED:
        callback();
        return;
      case FileMonitorResult.CANCELED:
        // Do nothing if it is canceled by another monitor call.
        return;
      case FileMonitorResult.ERROR:
        throw new Error('Error happens when monitoring file deletion');
      default:
        assertNotReached();
    }
  }

  async getDocumentScannerReadyState():
      Promise<{supported: boolean, ready: boolean}> {
    const {readyState} = await this.remote.getDocumentScannerReadyState();
    return {
      supported: readyState !== DocumentScannerReadyState.NOT_SUPPORTED,
      ready: readyState === DocumentScannerReadyState.SUPPORTED_AND_READY,
    };
  }

  /**
   * Checks the document mode readiness. Returns false if it fails to load.
   */
  async checkDocumentModeReadiness(): Promise<boolean> {
    const {isLoaded} = await this.remote.checkDocumentModeReadiness();
    return isLoaded;
  }

  /**
   * Scans the |blob| data and returns the detected document corners.
   *
   * @return Promise resolve to positions of document corner. Null for failing
   *     to detected corner positions.
   */
  async scanDocumentCorners(blob: Blob): Promise<Point[]|null> {
    const buffer = new Uint8Array(await blob.arrayBuffer());

    const {corners} =
        await this.remote.scanDocumentCorners(castToNumberArray(buffer));
    if (corners.length === 0) {
      return null;
    }
    return corners.map(({x, y}) => new Point(x, y));
  }

  /**
   * Converts the blob to document given by its |blob| data, |rotation| and
   * target |corners| to crop. The output will be converted according to given
   * |mimeType|.
   */
  async convertToDocument(
      blob: Blob, corners: Point[], rotation: number,
      mimeType: MimeType): Promise<Blob> {
    assert(corners.length === 4, 'Unexpected amount of corners');
    const buffer = new Uint8Array(await blob.arrayBuffer());
    let outputFormat;
    if (mimeType === MimeType.JPEG) {
      outputFormat = DocumentOutputFormat.JPEG;
    } else if (mimeType === MimeType.PDF) {
      outputFormat = DocumentOutputFormat.PDF;
    } else {
      throw new Error(`Output mimetype unsupported: ${mimeType}`);
    }

    const {docData} = await this.remote.convertToDocument(
        castToNumberArray(buffer), corners, castToMojoRotation(rotation),
        outputFormat);
    return new Blob([new Uint8Array(docData)], {type: mimeType});
  }

  /**
   * Converts given |jpegBlobs| to PDF format.
   *
   * @return Blob in PDF format.
   */
  async convertToPdf(jpegBlobs: Blob[]): Promise<Blob> {
    const numArrays = await Promise.all(jpegBlobs.map(async (blob) => {
      const buffer = new Uint8Array(await blob.arrayBuffer());
      return castToNumberArray(buffer);
    }));
    const {pdfData} = await this.remote.convertToPdf(numArrays);
    return new Blob([new Uint8Array(pdfData)], {type: MimeType.PDF});
  }

  /**
   * Tries to trigger HaTS survey for CCA.
   */
  maybeTriggerSurvey(): void {
    this.remote.maybeTriggerSurvey();
  }

  async startMonitorStorage(onChange: (status: StorageMonitorStatus) => void):
      Promise<StorageMonitorStatus> {
    const storageCallbackRouter =
        wrapEndpoint(new StorageMonitorCallbackRouter());
    storageCallbackRouter.update.addListener(
        (newStatus: StorageMonitorStatus) => {
          if (newStatus === StorageMonitorStatus.ERROR) {
            throw new Error('Error occurred while monitoring storage.');
          } else if (newStatus !== StorageMonitorStatus.CANCELED) {
            onChange(newStatus);
          }
        });

    const {initialStatus} = await this.remote.startStorageMonitor(
        storageCallbackRouter.$.bindNewPipeAndPassRemote());
    // Should not get canceled status at initial time.
    if (initialStatus === StorageMonitorStatus.ERROR ||
        initialStatus === StorageMonitorStatus.CANCELED) {
      throw new Error('Failed to start storage monitoring.');
    }
    return initialStatus;
  }

  stopMonitorStorage(): void {
    this.remote.stopStorageMonitor();
  }

  openStorageManagement(): void {
    this.remote.openStorageManagement();
  }

  /**
   * Creates a new instance of ChromeHelper if it is not set. Returns the
   *     existing instance.
   *
   * @return The singleton instance.
   */
  static getInstance(): ChromeHelper {
    if (instance === null) {
      instance = new ChromeHelper();
    }
    return instance;
  }
}
