/* eslint-disable no-undef */
/* eslint-disable no-unused-vars */
module.exports={
  'uds': [
    {
      name: 'DiagnosticSessionControl',
      value: 0x10,
      input: [
        {
          name: 'subFunction',
          type: 'subfunction',
          rule: [
            { required: true, message: 'Please Choose SubFunction', trigger: 'change' },
          ],
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
      ]
    },
    {

      name: 'ECUReset',
      value: 0x11,
      input: [
        {
          name: 'subFunction',
          type: 'subfunction',
          rule: [
            { required: true, message: 'Please Choose SubFunction', trigger: 'change' },
          ],
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
      ]
    },
    {

      name: 'SecurityAccess',
      value: 0x27,
      input: [
        {
          name: 'subFunction',
          type: 'subfunction',
          rule: [
            { required: true, message: 'Please Choose SubFunction', trigger: 'change' },
          ],
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
        {
          name: 'AccessData / securityKey',
          type: 'input',
          rule: [
            { pattern: "^([0-9a-fA-F]{2})+$", message: 'Please Input HEX Format', trigger: 'change' },
          ]
        }
      ]
    },
    {

      name: 'CommunicationControl',
      value: 0x28,
      input: [
        {
          name: 'subFunction',
          type: 'subfunction',
          rule: [
            { required: true, message: 'Please Choose SubFunction', trigger: 'change' },
          ],
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
        {
          name: 'communicationType',
          type: 'input',
          rule: [
            { pattern: "^([0-9a-fA-F]{2})+$", message: 'Please Input HEX Format', trigger: 'change' },
            { min: 2, max: 2, message: 'Length Need 1 Byte', trigger: 'change' }
          ]
        }
      ]
    },
    {

      name: 'TesterPresent',
      value: 0x3e,
      input: [
        {
          name: 'subFunction',
          type: 'subfunction',
          rule: [
            { required: true, message: 'Please Choose SubFunction', trigger: 'change' },
          ],
          options: [
            {
              name: 'zeroSubFunction',
              value: 0
            }
          ]
        }
      ]
    },
    {

      name: 'AccessTimingParameter',
      value: 0x83,
      input: [
        {
          name: 'subFunction',
          type: 'subfunction',
          rule: [
            { required: true, message: 'Please Choose SubFunction', trigger: 'change' },
          ],
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
        {
          name: 'TimingParameterRequestRecord',
          type: 'input',
          rule: [
            { pattern: "^([0-9a-fA-F]{2})+$", message: 'Please Input HEX Format', trigger: 'change' },
          ]
        }
      ]
    },
    {

      name: 'SecuredDataTransmission',
      value: 0x84,
      input: [
        {
          name: 'securityDataRequestRecord',
          type: 'input',
          rule: [
            { required: true, pattern: "^([0-9a-fA-F]{2})+$", message: 'Please Input HEX Format', trigger: 'change' }
          ]
        }
      ]
    },
    {

      name: 'ControlDTCSetting',
      value: 0x85,
      input: [
        {
          name: 'subFunction',
          type: 'subfunction',
          rule: [
            { required: true, message: 'Please Choose SubFunction', trigger: 'change' },
          ],
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
        }
      ]
    },
    {

      name: 'ResponseOnEvent',
      value: 0x86,
      input: [
        {
          name: 'subFunction',
          type: 'subfunction',
          rule: [
            { required: true, message: 'Please Choose SubFunction', trigger: 'change' },
          ],
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
        {
          name: 'eventWindowTime',
          type: 'input',
          rule: [
            { required: true, pattern: "^([0-9a-fA-F]{2})+$", message: 'Please Input HEX Format', trigger: 'change' },
            { min: 2, max: 2, message: 'Length Need 1 Byte', trigger: 'change' }
          ]
        },
        {
          name: 'eventTypeRecord',
          type: 'input',
          rule: [
            { pattern: "^([0-9a-fA-F]{2})+$", message: 'Please Input HEX Format', trigger: 'change' },
          ]
        }
      ]
    },
    {

      name: 'LinkControl',
      value: 0x87,
      input: [
        {
          name: 'subFunction',
          type: 'subfunction',
          rule: [
            { required: true, message: 'Please Choose SubFunction', trigger: 'change' },
          ],
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
        {
          name: 'baudrateIdentifier/linkBaudrateRecord',
          type: 'input',
          rule: [
            { pattern: "^([0-9a-fA-F]{2})+$", message: 'Please Input HEX Format', trigger: 'change' },
          ]
        }
      ]
    },
    {

      name: 'ReadDataByIdentifier',
      value: 0x22,
      input: [
        {
          name: 'dataIdentifier',
          type: 'input',
          rule: [
            { required: true, pattern: "^([0-9a-fA-F]{4})+$", message: 'Please Input HEX Format,Two Bytes As A Group', trigger: 'change' },
          ]
        }
      ]
    },
    {

      name: 'ReadMemoryByAddress',
      value: 0x23,
      input: [
        {
          name: 'addressAndLengthFormatIdentifier',
          type: 'input',
          rule: [
            { required: true, pattern: "^([0-9a-fA-F]{2})+$", message: 'Please Input HEX Format', trigger: 'change' },
            { min: 2, max: 2, message: 'Length Need 1 Byte', trigger: 'change' }
          ]
        },
        {
          name: 'memoryAddress',
          type: 'input',
          rule: [
            { required: true, pattern: "^([0-9a-fA-F]{2})+$", message: 'Please Input HEX Format', trigger: 'change' },
          ]
        },
        {
          name: 'memorySize',
          type: 'input',
          rule: [
            { required: true, pattern: "^([0-9a-fA-F]{2})+$", message: 'Please Input HEX Format', trigger: 'change' },
          ]
        }
      ]
    },
    {

      name: 'ReadScalingDataByIdentifier',
      value: 0x24,
      input: [
        {
          name: 'dataIdentifier',

          type: 'input',
          rule: [
            { required: true, pattern: "^([0-9a-fA-F]{2})+$", message: 'Please Input HEX Format', trigger: 'change' },
            { min: 2, max: 2, message: 'Length Need 1 Byte', trigger: 'change' }
          ]
        }
      ]
    },
    {

      name: 'ReadDataByPeriodicIdentifier',
      value: 0x2a,
      input: [
        {
          name: 'transmissionMode',

          type: 'input',
          rule: [
            { required: true, pattern: "^([0-9a-fA-F]{2})+$", message: 'Please Input HEX Format', trigger: 'change' },
            { min: 2, max: 2, message: 'Length Need 1 Byte', trigger: 'change' }
          ]
        },
        {
          name: 'periodicDataIdentifier',

          type: 'input',
          rule: [
            { pattern: "^([0-9a-fA-F]{2})+$", message: 'Please Input HEX Format', trigger: 'change' },
          ]
        }
      ]
    },
    {

      name: 'DynamicallyDefineDataIdentifier',
      value: 0x2c,
      input: [
        {
          name: 'subFunction',

          type: 'subfunction',
          rule: [
            { required: true, message: 'Please Choose SubFunction', trigger: 'change' },
          ],
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
        {
          name: 'dynamicallyDefinedDataIdentifier',

          type: 'input',
          rule: [
            { required: true, pattern: "^([0-9a-fA-F]{2})+$", message: 'Please Input HEX Format', trigger: 'change' },
            { min: 2, max: 2, message: 'Length Need 1 Byte', trigger: 'change' }
          ]
        }
      ]
    },
    {

      name: 'WriteDataByIdentifier',
      value: 0x2e,
      input: [
        {
          name: 'dataIdentifier',

          type: 'input',
          rule: [
            { required: true, pattern: "^([0-9a-fA-F]{2})+$", message: 'Please Input HEX Format', trigger: 'change' },
            { min: 4, max: 4, message: 'Length Need 2 Bytes', trigger: 'change' }
          ]
        },
        {
          name: 'dataRecord',

          type: 'input',
          rule: [
            { required: true, pattern: "^([0-9a-fA-F]{2})+$", message: 'Please Input HEX Format', trigger: 'change' },
          ]
        }
      ]
    },
    {

      name: 'WriteMemoryByAddress',
      value: 0x3d,
      input: [
        {
          name: 'addressAndLengthFormatIdentifier',

          type: 'input',
          rule: [
            { required: true, pattern: "^([0-9a-fA-F]{2})+$", message: 'Please Input HEX Format', trigger: 'change' },
            { min: 2, max: 2, message: 'Length Need 1 Byte', trigger: 'change' }
          ]
        },
        {
          name: 'memoryAddress',

          type: 'input',
          rule: [
            { required: true, pattern: "^([0-9a-fA-F]{2})+$", message: 'Please Input HEX Format', trigger: 'change' },
          ]
        },
        {
          name: 'memorySize',

          type: 'input',
          rule: [
            { required: true, pattern: "^([0-9a-fA-F]{2})+$", message: 'Please Input HEX Format', trigger: 'change' },
          ]
        },
        {
          name: 'dataRecord',

          type: 'text',
          rule: [
            { required: true, pattern: "^([0-9a-fA-F]{2})+$", message: 'Please Input HEX Format', trigger: 'change' },
          ]
        },
      ]
    },
    {

      name: 'ClearDiagnosticInformation',
      value: 0x14,
      input: [
        {
          name: 'groupOfDTC',

          type: 'input',
          rule: [
            { required: true, pattern: "^([0-9a-fA-F]{2})+$", message: 'Please Input HEX Format', trigger: 'change' },
            { min: 6, max: 6, message: 'Length Need 3 Bytes', trigger: 'change' }
          ]
        }
      ]
    },
    {

      name: 'ReadDTCInformation',
      value: 0x19,
      input: [
        {
          name: 'subFunction',

          type: 'subfunction',
          rule: [
            { required: true, message: 'Please Choose SubFunction', trigger: 'change' },
          ],
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
      ]
    },
    {

      name: 'InputOutputControlByIdentifier',
      value: 0x2f,
      input: [
        {
          name: 'dataIdentifier',

          type: 'input',
          rule: [
            { required: true, pattern: "^([0-9a-fA-F]{2})+$", message: 'Please Input HEX Format', trigger: 'change' },
            { min: 4, max: 4, message: 'Length Need 2 Bytes', trigger: 'change' }
          ]
        },
        {
          name: 'controlOptionRecord',

          type: 'input',
          rule: [
            { required: true, pattern: "^([0-9a-fA-F]{2})+$", message: 'Please Input HEX Format', trigger: 'change' },
          ]
        },
        {
          name: 'controlEnableMaskRecord',

          type: 'input',
          rule: [
            { pattern: "^([0-9a-fA-F]{2})+$", message: 'Please Input HEX Format', trigger: 'change' },
          ]
        }
      ]
    },
    {

      name: 'RoutineControl',
      value: 0x31,
      input: [
        {
          name: 'subFunction',

          type: 'subfunction',
          rule: [
            { required: true, message: 'Please Choose SubFunction', trigger: 'change' },
          ],
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
        {
          name: 'routineIdentifier',

          type: 'input',
          rule: [
            { required: true, pattern: "^([0-9a-fA-F]{2})+$", message: 'Please Input HEX Format', trigger: 'change' },
            { min: 4, max: 4, message: 'Length Need 2 Bytes', trigger: 'change' }
          ]
        },
        {
          name: 'routineControlOptionRecord',

          type: 'input',
          rule: [
            { pattern: "^([0-9a-fA-F]{2})+$", message: 'Please Input HEX Format', trigger: 'change' },
          ]
        },
        {
          name: 'routineControlOptionRecord-ascii',
          type: 'text',
        }
      ]
    },
    {

      name: 'RequestDownload',
      value: 0x34,
      input: [
        {
          name: 'Download File',
          type: 'downloadFile',
        },
        {
          name: 'dataFormatIdentifier',

          type: 'input',
          rule: [
            { required: true, pattern: "^([0-9a-fA-F]{2})+$", message: 'Please Input HEX Format', trigger: 'change' },
            { min: 2, max: 2, message: 'Length Need 1 Byte', trigger: 'change' }
          ]
        },
        {
          name: 'addressAndLengthFormatIdentifier',

          type: 'input',
          rule: [
            { required: true, pattern: "^([0-9a-fA-F]{2})+$", message: 'Please Input HEX Format', trigger: 'change' },
            { min: 2, max: 2, message: 'Length Need 1 Byte', trigger: 'change' }
          ]
        },
        {
          name: 'memoryAddress',

          type: 'input',
          rule: [
            { required: true, pattern: "^([0-9a-fA-F]{2})+$", message: 'Please Input HEX Format', trigger: 'change' },
          ]
        },
        {
          name: 'memorySize',

          type: 'input',
          rule: [
            { required: true, pattern: "^([0-9a-fA-F]{2})+$", message: 'Please Input HEX Format', trigger: 'change' },
          ]
        }
      ]
    },
    {

      name: 'RequestUpload',
      value: 0x35,
      input: [
        {
          name: 'Upload File',
          type: 'uploadFile',
        },
        {
          name: 'dataFormatIdentifier',

          type: 'input',
          rule: [
            { required: true, pattern: "^([0-9a-fA-F]{2})+$", message: 'Please Input HEX Format', trigger: 'change' },
            { min: 2, max: 2, message: 'Length Need 1 Byte', trigger: 'change' }
          ]
        },
        {
          name: 'addressAndLengthFormatIdentifier',

          type: 'input',
          rule: [
            { required: true, pattern: "^([0-9a-fA-F]{2})+$", message: 'Please Input HEX Format', trigger: 'change' },
            { min: 2, max: 2, message: 'Length Need 1 Byte', trigger: 'change' }
          ]
        },
        {
          name: 'memoryAddress',

          type: 'input',
          rule: [
            { required: true, pattern: "^([0-9a-fA-F]{2})+$", message: 'Please Input HEX Format', trigger: 'change' },
          ]
        },
        {
          name: 'memorySize',

          type: 'input',
          rule: [
            { required: true, pattern: "^([0-9a-fA-F]{2})+$", message: 'Please Input HEX Format', trigger: 'change' },
          ]
        }
      ]
    },
    {

      name: 'RequestTransferExit',
      value: 0x37,
      input: [
        {
          name: 'transferRequestParameterRecord',

          type: 'input',
          rule: [
            { pattern: "^([0-9a-fA-F]{2})+$", message: 'Please Input HEX Format', trigger: 'change' },
          ]
        },
      ]
    },
    {
      name: 'RequestFileTransfer',
      value: 0x38,
      input: [
        {
          name: 'Upload File',
          type: 'uploadFile',
        },
        {
          name: 'Download File',
          type: 'downloadFile',
        },
        {
          name: 'modeOfOperation',
          type: 'select',
          rule: [
            { required: true },
          ],
          options: [
            {
              name: 'AddFile',
              value: 1
            },
            {
              name: 'DeleteFile',
              value: 2
            },
            {
              name: 'ReplaceFile',
              value: 3
            },
            {
              name: 'ReadFile',
              value: 4
            },
            {
              name: 'ReadDir',
              value: 5
            }
          ]
        },
        {
          name: 'filePathAndNameLength',
          type: 'input',
          rule: [
            // eslint-disable-next-line no-useless-escape
            { required: true, pattern: "^(([0-9a-fA-F]{2}){2}|len\\(filePathAndName\\))$", message: 'Please Input HEX Format,or len(filePathAndName)', trigger: 'change' },
          ]
        },
        {
          name: 'filePathAndName',
          type: 'text',
          rule:[
            { required: true},
            { min: 1, }
          ]
        },
        {
          name: 'dataFormatIdentifier',
          type: 'input',
          rule:[
            { pattern: "^([0-9a-fA-F]{2})+$", message: 'Please Input HEX Format', trigger: 'change' },
            { max: 2, message: 'Length Need 1 Byte', trigger: 'change' }
          ]
        },
        {
          name: 'fileSizeParameterLength',
          type: 'input',
          rule:[
            { pattern: "^([0-9a-fA-F]{2})+$", message: 'Please Input HEX Format', trigger: 'change' },
            { min:4,max: 4, message: 'Length Need 2 Bytes', trigger: 'change' }
          ]
        },
        {
          name: 'fileSizeUnCompressed',
          type: 'input',
          rule:[
            { pattern: "^([0-9a-fA-F]{2})+$", message: 'Please Input HEX Format', trigger: 'change' },
          ]
        },
        {
          name: 'fileSizeCompressed',
          type: 'input',
          rule:[
            { pattern: "^(([0-9a-fA-F]{2})+|equal\\(fileSizeUnCompressed\\))$", message: 'Please Input HEX Format,or equal(fileSizeUnCompressed)', trigger: 'change' },
          ]
        }
      ]
    }
  ],
  'doip': [

  ]
}
