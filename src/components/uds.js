/* eslint-disable no-unused-vars */
export default {
  '10': {
    value: 0x10,
    name: 'DiagnosticSessionControl',
    subFunction: {
      name: 'diagnosticSessionType',
      options: [
        {
          name: 'defaultSession',
          value: 1
        },
        {
          name: 'programmingSession',
          value: 2
        },
        {
          name: 'extendedDiagnosticSession',
          value: 3
        },
        {
          name: 'safetySystemDiagnosticSession',
          value: 4
        }
      ]
    }
  },
  '11': {
    value: 0x11,
    name: 'ECUReset',
    subFunction: {
      name: 'resetType',
      options: [
        {
          name: 'hardReset',
          value: 1
        },
        {
          name: 'keyOffOnReset',
          value: 2
        },
        {
          name: 'softReset',
          value: 3
        },
        {
          name: 'enableRapidPowerShutDown',
          value: 4
        },
        {
          name: 'disableRapidPowerShutDown',
          value: 5
        }
      ]
    }
  },
  '27': {
    value: 0x27,
    name: 'SecurityAccess',
    subFunction: {
      name: 'securityAccessType',
      options: [
        {
          name: 'requestSeed1',
          value: 1
        }, {
          name: 'sendKey1',
          value: 2
        }, {
          name: 'requestSeed2',
          value: 3
        },
        {
          name: 'sendKey2',
          value: 4
        }, {
          name: 'requestSeed3',
          value: 5
        },
        {
          name: 'sendKey3',
          value: 6
        }, {
          name: 'requestSeed4',
          value: 7
        },
        {
          name: 'sendKey4',
          value: 8
        }
      ]
    },
    param: [
      {
        name: 'AccessData / securityKey',
        len: function (sub) {
          return [(sub % 2) ? 0 : 1, 65536]
        }
      }
    ]
  },
  '28': {
    value: 0x28,
    name: 'CommunicationControl',
    subFunction: {
      name: 'controlType',
      options: [
        {
          name: 'enableRxAndTx',
          value: 0
        }, {
          name: 'enableRxAndDisableTx',
          value: 1
        }, {
          name: 'disableRxAndEnableTx',
          value: 2
        },
        {
          name: 'disableRxAndTx',
          value: 3
        }
      ]
    },
    param: [
      {
        name: 'communicationType',
        len: function (sub) {
          return [1, 1]
        }
      }
    ]
  },
  '3e': {
    value: 0x3E,
    name: 'TesterPresent',
    subFunction: {
      name: 'zeroSubFunction',
      options: [
        {
          name: 'zeroSubFunction',
          value: 0
        }
      ]
    }
  },
  '83': {
    value: 0x83,
    name: 'AccessTimingParameter',
    subFunction: {
      name: 'timingParameterAccessType',
      options: [
        {
          name: 'readExtendedTimingParameterSet',
          value: 1
        },
        {
          name: 'setTimingParametersToDefaultValues',
          value: 2
        },
        {
          name: 'readCurrentlyActiveTimingParameters',
          value: 3
        },
        {
          name: 'setTimingParametersToGivenValues',
          value: 4
        }
      ]
    },
    param: [
      {
        name: 'TimingParameterRequestRecord',
        len: function (sub) {
          return [0, 65536]
        }
      }
    ]
  },
  '84': {
    value: 0x84,
    name: 'SecuredDataTransmission',
    param: [
      {
        name: 'securityDataRequestRecord',
        len: function (sub) {
          return [1, 65536]
        }
      }
    ]
  },
  '85': {
    value: 0x85,
    name: 'ControlDTCSetting',
    subFunction: {
      name: 'DTCSettingType',
      options: [
        {
          name: 'on',
          value: 1
        },
        {
          name: 'off',
          value: 2
        }
      ]
    },
    param: [
      {
        name: 'DTCSettingControlOptionRecord',
        len: function (sub) {
          return [0, 65536]
        }
      }
    ]
  },
  '86': {
    value: 0x86,
    name: 'ResponseOnEvent',
    subFunction: {
      name: 'eventType',
      options: [
        {
          name: 'doNotStoreEvent|stopResponseOnEvent',
          value: 0
        },
        {
          name: 'doNotStoreEvent|onDTCStatusChange',
          value: 1
        },
        {
          name: 'doNotStoreEvent|onDTCStatusChange',
          value: 2
        },
        {
          name: 'doNotStoreEvent|onChangeOfDataIdentifier',
          value: 3
        },
        {
          name: 'doNotStoreEvent|reportActivatedEvents',
          value: 4
        },
        {
          name: 'doNotStoreEvent|startResponseOnEvent',
          value: 5
        },
        {
          name: 'doNotStoreEvent|clearResponseOnEvent',
          value: 6
        },
        {
          name: 'doNotStoreEvent|onComparisonOfValues',
          value: 7
        },
        {
          name: 'storeEvent|stopResponseOnEvent',
          value: 0 | 0x40
        },
        {
          name: 'storeEvent|onDTCStatusChange',
          value: 1 | 0x40
        },
        {
          name: 'storeEvent|onDTCStatusChange',
          value: 2 | 0x40
        },
        {
          name: 'storeEvent|onChangeOfDataIdentifier',
          value: 3 | 0x40
        },
        {
          name: 'storeEvent|reportActivatedEvents',
          value: 4 | 0x40
        },
        {
          name: 'storeEvent|startResponseOnEvent',
          value: 5 | 0x40
        },
        {
          name: 'storeEvent|clearResponseOnEvent',
          value: 6 | 0x40
        },
        {
          name: 'storeEvent|onComparisonOfValues',
          value: 7 | 0x40
        }
      ]
    },
    param: [
      {
        name: 'eventWindowTime',
        len: function (sub) {
          return [1, 1]
        }
      },
      {
        name: 'eventTypeRecord',
        len: function (sub) {
          const m = [0, 1, 1, 2, 0, 0, 0, 10]
          return [m[sub & 0x3f], m[sub & 0x3f]]
        }
      },
      {
        name: 'serviceToRespondToRecord[]',
        len: function (sub) {
          return [0, 65536]
        }
      }
    ]
  },
  '87': {
    value: 0x87,
    name: 'LinkControl',
    subFunction: {
      name: 'LinkControl',
      options: [
        {
          name: 'verifyBaudrateTransitionWithFixedBaudrate',
          value: 1
        },
        {
          name: 'verifyBaudrateTransitionWithSpecificBaudrate',
          value: 2
        },
        {
          name: 'transitionBaudrate',
          value: 3
        }
      ]
    },
    param: [
      {
        name: 'baudrateIdentifier/linkBaudrateRecord',
        len: function (sub) {
          return [0, 3]
        }
      }
    ]
  },
  '22': {
    value: 0x22,
    name: 'ReadDataByIdentifier',
    param: [
      {
        name: 'dataIdentifier',
        len: function (sub) {
          return [2, 2]
        }
      }
    ]
  },
  '23': {
    value: 0x23,
    name: 'ReadMemoryByAddress',
    param: [
      {
        name: 'addressAndLengthFormatIdentifier',
        len: function (sub) {
          return [1, 1]
        }
      },
      {
        name: 'memoryAddress',
        len: function (sub) {
          return [1, 16]
        }
      },
      {
        name: 'memorySize',
        len: function (sub) {
          return [1, 16]
        }
      }

    ]
  },
  '24': {
    value: 0x24,
    name: 'ReadScalingDataByIdentifier',
    param: [
      {
        name: 'dataIdentifier',
        len: function (sub) {
          return [2, 2]
        }
      }
    ]
  },
  '2a': {
    value: 0x2a,
    name: 'ReadDataByPeriodicIdentifier',
    param: [
      {
        name: 'transmissionMode',
        len: function (sub) {
          return [1, 1]
        }
      },
      {
        name: 'periodicDataIdentifier',
        len: function (sub) {
          return [0, 65536]
        }
      }
    ]
  },
  '2c': {
    value: 0x2c,
    name: 'DynamicallyDefineDataIdentifier',
    subFunction: {
      name: 'defineByIdentifier',
      options: [
        {
          name: 'defineByIdentifier',
          value: 1
        },
        {
          name: 'defineByMemoryAddress',
          value: 2
        },
        {
          name: 'clearDynamicallyDefinedDataIdentifier',
          value: 3
        }
      ]
    },
    param: [
      {
        name: 'dynamicallyDefinedDataIdentifier',
        len: function (sub) {
          return [2, 2]
        }
      }
    ]
  },
  '2e': {
    value: 0x2e,
    name: 'WriteDataByIdentifier',
    param: [
      {
        name: 'dataIdentifier',
        len: function (sub) {
          return [2, 2]
        }
      },
      {
        name: 'dataRecord',
        len: function (sub) {
          return [1, 65536]
        }
      }
    ]
  },
  '3d': {
    value: 0x3d,
    name: 'WriteMemoryByAddress',
    param: [
      {
        name: 'addressAndLengthFormatIdentifier',
        len: function (sub) {
          return [1, 1]
        }
      },
      {
        name: 'memoryAddress',
        len: function (sub) {
          return [1, 16]
        }
      },
      {
        name: 'memorySize',
        len: function (sub) {
          return [1, 16]
        }
      },
      {
        name: 'dataRecord',
        len: function (sub) {
          return [1, 65536]
        }
      }
    ]
  },
  '14': {
    value: 0x14,
    name: 'ClearDiagnosticInformation',
    param: [
      {
        name: 'groupOfDTC',
        len: function (sub) {
          return [3, 3]
        }
      }
    ]
  },
  '19': {
    value: 0x19,
    name: 'ReadDTCInformation',
    subFunction: {
      name: 'subFunction',
      options: [
        {
          name: 'reportNumberOfDTCByStatusMask',
          value: 1
        },
        {
          name: 'reportDTCByStatusMask',
          value: 2
        },
        {
          name: 'reportDTCSnapshotIdentification',
          value: 3
        },
        {
          name: 'reportDTCSnapshotRecordByDTCNumber',
          value: 4
        },
        {
          name: 'reportDTCSnapshotRecordByRecordNumber',
          value: 5
        },
        {
          name: 'reportDTCExtendedDataRecordByDTCNumber',
          value: 6
        },
        {
          name: 'reportNumberOfDTCBySeverityMaskRecord',
          value: 7
        },
        {
          name: 'reportDTCBySeverityMaskRecord',
          value: 8
        },
        {
          name: 'reportSeverityInformationOfDTC',
          value: 9
        },
        {
          name: 'reportSupportedDTC',
          value: 10
        },
        {
          name: 'reportFirstTestFailedDTC',
          value: 11
        },
        {
          name: 'reportFirstConfirmedDTC',
          value: 12
        },
        {
          name: 'reportMostRecentTestFailedDTC',
          value: 13
        },
        {
          name: 'reportMostRecentConfirmedDTC',
          value: 14
        },
        {
          name: 'reportMirrorMemoryDTCByStatusMask',
          value: 15
        },
        {
          name: 'reportMirrorMemoryDTCExtendedDataRecordByDTCNumber',
          value: 16
        },
        {
          name: 'reportNumberOfMirrorMemoryDTCByStatusMask',
          value: 17
        },
        {
          name: 'reportNumberOfEmissionsRelatedOBDDTCByStatusMask',
          value: 18
        },
        {
          name: 'reportEmissionsRelatedOBDDTCByStatusMask',
          value: 19
        },
        {
          name: 'reportDTCFaultDetectionCounter',
          value: 20
        },
        {
          name: 'reportDTCWithPermanentStatus',
          value: 21
        }
      ]
    }
  },
  '2f': {
    value: 0x2f,
    name: 'InputOutputControlByIdentifier',
    param: [
      {
        name: 'dataIdentifier',
        len: function (sub) {
          return [2, 2]
        }
      },
      {
        name: 'controlOptionRecord',
        len: function (sub) {
          return [1, 65536]
        }
      },
      {
        name: 'controlEnableMaskRecord',
        len: function (sub) {
          return [0, 65536]
        }
      }
    ]
  },
  '31': {
    value: 0x31,
    name: 'RoutineControl',
    subFunction: {
      name: 'routineControlType',
      options: [
        {
          name: 'startRoutine',
          value: 1
        },
        {
          name: 'stopRoutine',
          value: 2
        },
        {
          name: 'requestRoutineResults',
          value: 3
        }
      ]
    },
    param: [
      {
        name: 'routineIdentifier',
        len: function (sub) {
          return [2, 2]
        }
      },
      {
        name: 'routineControlOptionRecord',
        len: function (sub) {
          return [0, 65536]
        }
      }
    ]
  },
  '34': {
    value: 0x34,
    name: 'RequestDownload',
    param: [
      {
        name: 'dataFormatIdentifier',
        len: function (sub) {
          return [1, 1]
        }
      },
      {
        name: 'addressAndLengthFormatIdentifier',
        len: function (sub) {
          return [1, 1]
        }
      },
      {
        name: 'memoryAddress',
        len: function (sub) {
          return [1, 16]
        }
      },
      {
        name: 'memorySize',
        len: function (sub) {
          return [1, 16]
        }
      }
    ],
    file: 'download'
  },
  '35': {
    value: 0x35,
    name: 'RequestUpload',
    param: [
      {
        name: 'dataFormatIdentifier',
        len: function (sub) {
          return [1, 1]
        }
      },
      {
        name: 'addressAndLengthFormatIdentifier',
        len: function (sub) {
          return [1, 1]
        }
      },
      {
        name: 'memoryAddress',
        len: function (sub) {
          return [1, 16]
        }
      },
      {
        name: 'memorySize',
        len: function (sub) {
          return [1, 16]
        }
      }
    ],
    file: 'upload'
  },
  '37': {
    value: 0x37,
    name: 'RequestTransferExit',
    param: [
      {
        name: 'transferRequestParameterRecord[]',
        len: function (sub) {
          return [0, 65536]
        }
      }
    ]
  }
}
