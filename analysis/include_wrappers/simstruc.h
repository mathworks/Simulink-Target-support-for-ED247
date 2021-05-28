#ifndef __MW_WRAPPER_SIMSTRUC_H__
#define __MW_WRAPPER_SIMSTRUC_H__
#include "real_simstruc.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */
#undef ssSetNumInputPorts
extern boolean_T ssSetNumInputPorts(SimStruct* S, int_T num);

#undef ssGetInputPortComplexSignal
extern CSignal_T ssGetInputPortComplexSignal(SimStruct* S, int_T pIdx);

#undef ssSetInputPortComplexSignal
extern CSignal_T ssSetInputPortComplexSignal(SimStruct* S, int_T pIdx, CSignal_T csig);

#undef ssGetInputPortDataType
extern DTypeId ssGetInputPortDataType(SimStruct* S, int_T pIdx);

#undef ssSetInputPortDataType
extern DTypeId ssSetInputPortDataType(SimStruct* S, int_T pIdx, DTypeId id);

#undef ssGetInputPortDirectFeedThrough
extern int_T ssGetInputPortDirectFeedThrough(SimStruct* S, int_T pIdx);

#undef ssSetInputPortDirectFeedThrough
extern void ssSetInputPortDirectFeedThrough(SimStruct* S, int_T pIdx, int_T dirFeed);

#undef ssGetInputPortOffsetTime
extern real_T ssGetInputPortOffsetTime(SimStruct* S, int_T pIdx);

#undef ssSetInputPortOffsetTime
extern real_T ssSetInputPortOffsetTime(SimStruct* S, int_T pIdx, real_T offset);

#undef ssGetInputPortRequiredContiguous
extern int_T ssGetInputPortRequiredContiguous(SimStruct* S, int_T pIdx);

#undef ssSetInputPortRequiredContiguous
extern void ssSetInputPortRequiredContiguous(SimStruct* S, int_T pIdx, int_T flag);

#undef ssGetInputPortSampleTime
extern real_T ssGetInputPortSampleTime(SimStruct* S, int_T pIdx);

#undef ssSetInputPortSampleTime
extern real_T ssSetInputPortSampleTime(SimStruct* S, int_T pIdx, real_T period);

#undef ssGetInputPortSampleTimeIndex
extern int_T ssGetInputPortSampleTimeIndex(SimStruct* S, int_T pIdx);

#undef ssGetInputPortFrameData
extern Frame_T ssGetInputPortFrameData(SimStruct* S, int_T pIdx);

#undef ssSetInputPortFrameData
extern void ssSetInputPortFrameData(SimStruct* S, int_T pIdx, Frame_T frameData);

#undef ssSetZeroBasedIndexInputPort
extern void ssSetZeroBasedIndexInputPort(SimStruct* S, int_T pIdx);

#undef ssSetOneBasedIndexInputPort
extern void ssSetOneBasedIndexInputPort(SimStruct* S, int_T pIdx);

#undef ssSetNumOutputPorts
extern boolean_T ssSetNumOutputPorts(SimStruct* S, int_T num);

#undef ssGetOutputPortComplexSignal
extern CSignal_T ssGetOutputPortComplexSignal(SimStruct* S, int_T pIdx);

#undef ssSetOutputPortComplexSignal
extern CSignal_T ssSetOutputPortComplexSignal(SimStruct* S, int_T pIdx, CSignal_T csig);

#undef ssGetOutputPortDataType
extern DTypeId ssGetOutputPortDataType(SimStruct* S, int_T pIdx);

#undef ssSetOutputPortDataType
extern DTypeId ssSetOutputPortDataType(SimStruct* S, int_T pIdx, DTypeId id);

#undef ssGetOutputPortOffsetTime
extern real_T ssGetOutputPortOffsetTime(SimStruct* S, int_T pIdx);

#undef ssSetOutputPortOffsetTime
extern real_T ssSetOutputPortOffsetTime(SimStruct* S, int_T pIdx, real_T offset);

#undef ssGetOutputPortSampleTime
extern real_T ssGetOutputPortSampleTime(SimStruct* S, int_T pIdx);

#undef ssSetOutputPortSampleTime
extern real_T ssSetOutputPortSampleTime(SimStruct* S, int_T pIdx, real_T period);

#undef ssGetOutputPortSampleTimeIndex
extern int_T ssGetOutputPortSampleTimeIndex(SimStruct* S, int_T pIdx);

#undef ssGetOutputPortFrameData
extern Frame_T ssGetOutputPortFrameData(SimStruct* S, int_T pIdx);

#undef ssSetOutputPortFrameData
extern void ssSetOutputPortFrameData(SimStruct* S, int_T pIdx, Frame_T frameData);

#undef ssSetZeroBasedIndexOutputPort
extern void ssSetZeroBasedIndexOutputPort(SimStruct* S, int_T pIdx);

#undef ssSetOneBasedIndexOutputPort
extern void ssSetOneBasedIndexOutputPort(SimStruct* S, int_T pIdx);

#undef ssAllowSignalsWithMoreThan2D
extern void ssAllowSignalsWithMoreThan2D(SimStruct* S);

#undef ssSetInputPortDimensionInfo
extern int_T ssSetInputPortDimensionInfo(SimStruct* S,  int_T pIdx, DimsInfo_T* dimsInfo);

#undef ssGetInputPortDimensionsMode
extern DimensionsMode_T ssGetInputPortDimensionsMode(SimStruct* S, int_T pIdx);

#undef ssSetInputPortDimensionsMode
extern void ssSetInputPortDimensionsMode(SimStruct* S, int_T pIdx, DimensionsMode_T mode);

#undef ssSetInputPortDimsSameAsOutputPortDims
extern void ssSetInputPortDimsSameAsOutputPortDims(SimStruct* S, int_T inpIdx, int_T outIdx);

#undef ssSetInputPortMatrixDimensions
extern int_T ssSetInputPortMatrixDimensions(SimStruct* S, int_T pIdx, int_T m, int_T n);

#undef ssSetInputPortVectorDimension
extern int_T ssSetInputPortVectorDimension(SimStruct* S, int_T pIdx, int_T w);

#undef ssGetInputPortWidth
extern int_T ssGetInputPortWidth(SimStruct* S, int_T pIdx);

#undef ssSetInputPortWidth
extern int_T ssSetInputPortWidth(SimStruct* S, int_T pIdx, int_T width);

#undef ssPruneNDMatrixSingletonDims
extern void ssPruneNDMatrixSingletonDims(SimStruct* S, DimsInfo_T* dimsInfo);

#undef ssGetInputPortDimensions
extern int_T* ssGetInputPortDimensions(SimStruct* S, int_T pIdx);

#undef ssGetInputPortDimensionSize
extern int_T ssGetInputPortDimensionSize(SimStruct* S, int_T pIdx, int_T dIdx);

#undef ssGetInputPortNumDimensions
extern int_T ssGetInputPortNumDimensions(SimStruct* S, int_T pIdx);

#undef ssGetCurrentInputPortDimensions
extern int_T ssGetCurrentInputPortDimensions(SimStruct* S, int_T pIdx, int_T dIdx);

#undef ssGetCurrentInputPortWidth
extern int_T ssGetCurrentInputPortWidth(SimStruct* S, int_T pIdx);

#undef ssSetOutputPortDimensionInfo
extern int_T ssSetOutputPortDimensionInfo(SimStruct* S,  int_T pIdx, DimsInfo_T* dimsInfo);

#undef ssGetOutputPortDimensionsMode
extern DimensionsMode_T ssGetOutputPortDimensionsMode(SimStruct* S, int_T pIdx);

#undef ssSetOutputPortDimensionsMode
extern void ssSetOutputPortDimensionsMode(SimStruct* S, int_T pIdx, DimensionsMode_T mode);

#undef ssSetOutputPortMatrixDimensions
extern int_T ssSetOutputPortMatrixDimensions(SimStruct* S, int_T pIdx, int_T m, int_T n);

#undef ssSetOutputPortVectorDimension
extern int_T ssSetOutputPortVectorDimension(SimStruct* S, int_T pIdx, int_T w);

#undef ssGetOutputPortWidth
extern int_T ssGetOutputPortWidth(SimStruct* S, int_T pIdx);

#undef ssSetOutputPortWidth
extern int_T ssSetOutputPortWidth(SimStruct* S, int_T pIdx, int_T width);

#undef ssGetOutputPortDimensions
extern int_T* ssGetOutputPortDimensions(SimStruct* S, int_T pIdx);

#undef ssGetOutputPortDimensionSize
extern int_T ssGetOutputPortDimensionSize(SimStruct* S, int_T pIdx, int_T dIdx);

#undef ssGetOutputPortNumDimensions
extern int_T ssGetOutputPortNumDimensions(SimStruct* S, int_T pIdx);

#undef ssSetCurrentOutputPortDimensions
extern void ssSetCurrentOutputPortDimensions(SimStruct* S, int_T pIdx, int_T dIdx, int_T val);

#undef ssGetCurrentOutputPortDimensions
extern int_T ssGetCurrentOutputPortDimensions(SimStruct* S, int_T pIdx, int_T dIdx);

#undef ssGetCurrentOutputPortWidth
extern int_T ssGetCurrentOutputPortWidth(SimStruct* S, int_T pIdx);

#undef ssAddOutputDimsDependencyRule
extern void ssAddOutputDimsDependencyRule(SimStruct* S, int_T outIdx, DimsDependInfo_T* ruleInfo);

#undef ssAddVariableSizeSignalsRuntimeChecker
extern void ssAddVariableSizeSignalsRuntimeChecker(SimStruct* S, RuntimeCheckerType_T type);

#undef ssRegMdlSetInputPortDimensionsModeFcn
extern void ssRegMdlSetInputPortDimensionsModeFcn(SimStruct* S, mdlSetInputPortDimensionsModeFcn fcn);

#undef ssSetDWorkRequireResetForSignalSize
extern void ssSetDWorkRequireResetForSignalSize(SimStruct* S, int dIndex, SS_VariableSizeStateResetType type);

#undef ssSetSignalSizesComputeType
extern void ssSetSignalSizesComputeType(SimStruct* S, SS_VariableSizeComputeType type);

#undef ssGetNumInputPorts
extern int_T ssGetNumInputPorts(SimStruct* S);

#undef ssGetNumOutputPorts
extern int_T ssGetNumOutputPorts(SimStruct* S);

#undef ssGetInputPortBufferDstPort
extern int_T ssGetInputPortBufferDstPort(SimStruct* S, int_T pIdx);

#undef ssGetInputPortConnected
extern int_T ssGetInputPortConnected(SimStruct* S, int_T pIdx);

#undef ssGetInputPortOptimOpts
extern uint_T ssGetInputPortOptimOpts(SimStruct* S, int_T pIdx);

#undef ssSetInputPortOptimOpts
extern void ssSetInputPortOptimOpts(SimStruct* S, int_T pIdx, uint_T val);

#undef ssGetInputPortOverWritable
extern int_T ssGetInputPortOverWritable(SimStruct* S, int_T pIdx);

#undef ssSetInputPortOverWritable
extern void ssSetInputPortOverWritable(SimStruct* S, int_T pIdx, int_T isOverwritable);

#undef ssGetInputPortRealSignal
extern const real_T* ssGetInputPortRealSignal(SimStruct* S, int_T pIdx);

#undef ssGetInputPortRealSignalPtrs
extern InputRealPtrsType ssGetInputPortRealSignalPtrs(SimStruct* S, int_T pIdx);

#undef ssGetInputPortSignal
extern const void* ssGetInputPortSignal(SimStruct* S, int_T pIdx);

#undef ssGetInputPortSignalPtrs
extern InputPtrsType ssGetInputPortSignalPtrs(SimStruct* S, int_T pIdx);

#undef ssGetOutputPortConnected
extern int_T ssGetOutputPortConnected(SimStruct* S, int_T pIdx);

#undef ssGetOutputPortBeingMerged
extern int_T ssGetOutputPortBeingMerged(SimStruct* S, int_T pIdx);

#undef ssGetOutputPortOptimOpts
extern uint_T ssGetOutputPortOptimOpts(SimStruct* S, int_T pIdx);

#undef ssSetOutputPortOptimOpts
extern void ssSetOutputPortOptimOpts(SimStruct* S, int_T pIdx, uint_T val);

#undef ssGetOutputPortRealSignal
extern real_T* ssGetOutputPortRealSignal(SimStruct* S, int_T pIdx);

#undef ssGetOutputPortSignal
extern void * ssGetOutputPortSignal(SimStruct* S, int_T pIdx);

#undef ssSetOutputPortOverwritesInputPort
extern void ssSetOutputPortOverwritesInputPort(SimStruct* S, int_T outIdx, int_T inIdx);

#undef ssGetNumSampleTimes
extern int_T ssGetNumSampleTimes(SimStruct* S);

#undef ssSetNumSampleTimes
extern int_T ssSetNumSampleTimes(SimStruct* S, int_T num);

#undef ssSetSampleTime
extern time_T ssSetSampleTime(SimStruct* S, int_T sti, time_T period);

#undef ssGetSampleTime
extern time_T ssGetSampleTime(SimStruct* S, int_T sti);

#undef ssGetOffsetTime
extern time_T ssGetOffsetTime(SimStruct* S, int_T sti);

#undef ssSetOffsetTime
extern time_T ssSetOffsetTime(SimStruct* S, int_T sti, time_T offset);

#undef ssGetTNext
extern time_T ssGetTNext(SimStruct* S);

#undef ssSetTNext
extern void ssSetTNext(SimStruct* S, time_T tnext);

#undef ssSetModelReferenceSampleTimeDefaultInheritance
extern void ssSetModelReferenceSampleTimeDefaultInheritance(SimStruct* S);

#undef ssSetModelReferenceSampleTimeDisallowInheritance
extern void ssSetModelReferenceSampleTimeDisallowInheritance(SimStruct* S);

#undef ssSetModelReferenceSampleTimeInheritanceRule
extern void ssSetModelReferenceSampleTimeInheritanceRule(SimStruct* S, int_T rule);

#undef ssGetPortBasedSampleTimeBlockIsTriggered
extern boolean_T ssGetPortBasedSampleTimeBlockIsTriggered(SimStruct* S);

#undef ssSetParameterTuningCompliance
extern void ssSetParameterTuningCompliance(SimStruct* S, boolean_T flag);

#undef ssSampleAndOffsetAreTriggered
extern boolean_T ssSampleAndOffsetAreTriggered(real_T st, real_T ot);

#undef ssSampleAndOffsetAreTriggeredOrAsync
extern boolean_T ssSampleAndOffsetAreTriggeredOrAsync(real_T st, real_T ot);

#undef ssIsContinuousTask
extern boolean_T ssIsContinuousTask(SimStruct* S, int_T tid);

#undef ssIsSampleHit
extern boolean_T ssIsSampleHit(SimStruct* S, int_T sti, int_T tid);

#undef ssIsSpecialSampleHit
extern boolean_T ssIsSpecialSampleHit(SimStruct* S, int_T sti1, int_T sti2, int_T tid);

#undef ssGetDataTypeName
extern const char* ssGetDataTypeName(SimStruct* S, DTypeId id);

#undef ssGetNumDataTypes
extern int_T ssGetNumDataTypes(SimStruct* S);

#undef ssRegisterDataType
extern DTypeId ssRegisterDataType(SimStruct* S, const char* name);

#undef ssRegisterTypeFromNamedObject
extern void ssRegisterTypeFromNamedObject(SimStruct* S, const char* name, DTypeId* id);

#undef ssRegisterTypeFromExpr
extern void ssRegisterTypeFromExpr(SimStruct* S, const char* name, DTypeId* id);

#undef ssRegisterTypeFromExprNoError
extern void ssRegisterTypeFromExprNoError(SimStruct* S, const char* name, DTypeId* id);

#undef ssRegisterTypeFromParameter
extern void ssRegisterTypeFromParameter(SimStruct* S, int_T pIdx, DTypeId* id);

#undef ssSetDataTypeSize
extern int_T ssSetDataTypeSize(SimStruct* S, DTypeId id, int_T size);

#undef ssGetDataTypeSize
extern int_T ssGetDataTypeSize(SimStruct* S, DTypeId id);

#undef ssSetDataTypeZero
extern int_T ssSetDataTypeZero(SimStruct* S, DTypeId id, void* zero);

#undef ssGetDataTypeZero
extern const void* ssGetDataTypeZero(SimStruct* S, DTypeId id);

#undef ssGetDataTypeId
extern DTypeId ssGetDataTypeId(SimStruct* S, const char* name);

#undef ssGetDataTypeIdAliasedThruTo
extern DTypeId ssGetDataTypeIdAliasedThruTo(SimStruct* S, DTypeId id);

#undef ssSetBusPropagationSupported
extern void ssSetBusPropagationSupported(SimStruct* S, boolean_T val);

#undef ssSetBusOutputObjectName
extern void ssSetBusOutputObjectName(SimStruct* S, int_T portNum, char* busName);

#undef ssSetBusInputAsStruct
extern void ssSetBusInputAsStruct(SimStruct* S, int_T portNum, boolean_T val);

#undef ssSetBusOutputAsStruct
extern void ssSetBusOutputAsStruct(SimStruct* S, int_T portNum, boolean_T val);

#undef ssGetNumBusElements
extern int_T ssGetNumBusElements(SimStruct* S, int_T busTypeID);

#undef ssGetBusElementOffset
extern int_T ssGetBusElementOffset(SimStruct* S, int_T busTypeID, int_T elemIdx);

#undef ssGetBusElementDataType
extern int_T ssGetBusElementDataType(SimStruct* S, int_T busTypeID, int_T elemIdx);

#undef ssGetBusElementComplexSignal
extern CSignal_T ssGetBusElementComplexSignal(SimStruct* S, int_T busTypeID, int_T elemIdx);

#undef ssGetBusElementNumDimensions
extern int_T ssGetBusElementNumDimensions(SimStruct* S, int_T busTypeID, int_T elemIdx);

#undef ssGetBusElementDimensions
extern const void* ssGetBusElementDimensions(SimStruct* S, int_T busTypeID, int_T elemIdx);

#undef ssGetBusElementName
extern const void* ssGetBusElementName(SimStruct* S, int_T busTypeId, int_T elemIdx);

#undef ssIsDataTypeABus
extern boolean_T ssIsDataTypeABus(SimStruct* S, int_T id);

#undef ssGetNumSFcnParams
extern int_T ssGetNumSFcnParams(SimStruct* S);

#undef ssSetNumSFcnParams
extern int_T ssSetNumSFcnParams(SimStruct* S, int_T num);

#undef ssGetSFcnParam
extern const mxArray * ssGetSFcnParam(SimStruct* S, int_T index);

#undef ssGetSFcnParamsCount
extern int_T ssGetSFcnParamsCount(SimStruct* S);

#undef ssGetSFcnParamName
extern void ssGetSFcnParamName(SimStruct* S, int pIdx, char* value);

#undef ssSetSFcnParamNotTunable
extern void ssSetSFcnParamNotTunable(SimStruct* S, int_T index);

#undef ssSetSFcnParamTunable
extern void ssSetSFcnParamTunable(SimStruct* S, int_T param, int_T isTunable);

#undef ssCallSystemWithTid
extern int_T ssCallSystemWithTid(SimStruct* S, int_T element, int_T tid);

#undef ssDisableSystemWithTid
extern int_T ssDisableSystemWithTid(SimStruct* S, int_T element, int_T tid);

#undef ssEnableSystemWithTid
extern int_T ssEnableSystemWithTid(SimStruct* S, int_T element, int_T tid);

#undef ssGetCallSystemNumFcnCallDestinations
extern int_T ssGetCallSystemNumFcnCallDestinations(SimStruct* S, int_T elemIdx);

#undef ssSetCallSystemOutput
extern void ssSetCallSystemOutput(SimStruct* S, int_T element_index);

#undef ssGetExplicitFCSSCtrl
extern uint_T ssGetExplicitFCSSCtrl(SimStruct* S);

#undef ssSetExplicitFCSSCtrl
extern void ssSetExplicitFCSSCtrl(SimStruct* S, uint_T val);

#undef ssGetModelReferenceNormalModeSupport
extern ssModelReferenceNormalModeSupport ssGetModelReferenceNormalModeSupport(SimStruct* S);

#undef ssSetModelReferenceNormalModeSupport
extern void ssSetModelReferenceNormalModeSupport(SimStruct* S, ssModelReferenceNormalModeSupport mode);

#undef ssRTWGenIsModelReferenceRTWTarget
extern boolean_T ssRTWGenIsModelReferenceRTWTarget(SimStruct* S);

#undef ssRTWGenIsModelReferenceSimTarget
extern boolean_T ssRTWGenIsModelReferenceSimTarget(SimStruct* S);

#undef ssSetStateAbsTol
extern void ssSetStateAbsTol(SimStruct* S, const int_T idx, const real_T val);

#undef ssGetStateAbsTol
extern const real_T ssGetStateAbsTol(SimStruct* S, const int_T idx);

#undef ssGetBlockReduction
extern uint_T ssGetBlockReduction(SimStruct* S);

#undef ssSetBlockReduction
extern uint_T ssSetBlockReduction(SimStruct* S, uint_T n);

#undef ssSetSimStateCompliance
extern void ssSetSimStateCompliance(SimStruct* S, uint_T val);

#undef ssSetSimStateVisibility
extern void ssSetSimStateVisibility(SimStruct* S, uint_T val);

#undef ssSetSolverNeedsReset
extern void ssSetSolverNeedsReset(SimStruct* S);

#undef ssSetStopRequested
extern void ssSetStopRequested(SimStruct* S, int_T val);

#undef ssGetStopRequested
extern int_T ssGetStopRequested(SimStruct* S);

#undef ssGetFixedStepSize
extern time_T ssGetFixedStepSize(SimStruct* S);

#undef ssGetSimMode
extern SS_SimMode ssGetSimMode(SimStruct* S);

#undef ssGetSimStatus
extern void ssGetSimStatus(SimStruct* S, SS_SimStatus* status);

#undef ssGetSolverMode
extern SolverMode ssGetSolverMode(SimStruct* S);

#undef ssGetSolverName
extern const char_T* ssGetSolverName(SimStruct* S);

#undef ssGetT
extern time_T ssGetT(SimStruct* S);

#undef ssGetTaskTime
extern time_T ssGetTaskTime(SimStruct* S, int_T sti);

#undef ssGetTFinal
extern time_T ssGetTFinal(SimStruct* S);

#undef ssGetTStart
extern time_T ssGetTStart(SimStruct* S);

#undef ssIsExternalSim
extern boolean_T ssIsExternalSim(SimStruct* S);

#undef ssIsFirstInitCond
extern boolean_T ssIsFirstInitCond(SimStruct* S);

#undef ssIsMajorTimeStep
extern boolean_T ssIsMajorTimeStep(SimStruct* S);

#undef ssIsMinorTimeStep
extern boolean_T ssIsMinorTimeStep(SimStruct* S);

#undef ssIsVariableStepSolver
extern boolean_T ssIsVariableStepSolver(SimStruct* S);

#undef ssRTWGenIsAccelerator
extern boolean_T ssRTWGenIsAccelerator(SimStruct* S);

#undef ssIsRapidAcceleratorActive
extern boolean_T ssIsRapidAcceleratorActive(SimStruct* S);

#undef ssGetNeedAbsoluteTime
extern uint_T ssGetNeedAbsoluteTime(SimStruct* S);

#undef ssSetNeedAbsoluteTime
extern void ssSetNeedAbsoluteTime(SimStruct* S, uint_T n);

#undef ssGetNeedElapseTime
extern uint_T ssGetNeedElapseTime(SimStruct* S);

#undef ssSetNeedElapseTime
extern void ssSetNeedElapseTime(SimStruct* S, uint_T n);

#undef ssGetNumRunTimeParams
extern int_T ssGetNumRunTimeParams(SimStruct* S);

#undef ssSetNumRunTimeParams
extern boolean_T ssSetNumRunTimeParams(SimStruct* S, int_T num);

#undef ssGetRunTimeParamInfo
extern ssParamRec* ssGetRunTimeParamInfo(SimStruct* S, int_T param);

#undef ssSetRunTimeParamInfo
extern void ssSetRunTimeParamInfo(SimStruct* S, int_T param, ssParamRec* info);

#undef ssRegAllTunableParamsAsRunTimeParams
extern void ssRegAllTunableParamsAsRunTimeParams(SimStruct* S, const char_T* names[]);

#undef ssRegDlgParamAsRunTimeParam
extern void ssRegDlgParamAsRunTimeParam(SimStruct* S, int_T dlgIdx, int_T rtIdx, const char_T* name, DTypeId dtId);

#undef ssUpdateAllTunableParamsAsRunTimeParams
extern void ssUpdateAllTunableParamsAsRunTimeParams(SimStruct* S);

#undef ssUpdateDlgParamAsRunTimeParam
extern void ssUpdateDlgParamAsRunTimeParam(SimStruct* S, int_T rtIdx);

#undef ssUpdateRunTimeParamInfo
extern void ssUpdateRunTimeParamInfo(SimStruct* S, int_T param, ssParamRec *info);

#undef ssUpdateRunTimeParamData
extern void ssUpdateRunTimeParamData(SimStruct* S, int_T param, void *data);

#undef ssCheckSFcnParamValueAttribs
extern void ssCheckSFcnParamValueAttribs(SimStruct* S, int_T pIdx, const char* pName, DTypeId dtId, int_T nDims, int_T* dimsPtr, CSignal_T cplx);

#undef ssSetNumDWork
extern boolean_T ssSetNumDWork(SimStruct* S, int_T nDWork);

#undef ssSetDWorkComplexSignal
extern CSignal_T ssSetDWorkComplexSignal(SimStruct* S, int_T vector, CSignal_T csig);

#undef ssSetDWorkDataType
extern DTypeId ssSetDWorkDataType(SimStruct* S, int_T vector, DTypeId dtID);

#undef ssSetDWorkName
extern const char_T* ssSetDWorkName(SimStruct* S, int_T vector, char_T* name);

#undef ssSetDWorkUsageType
extern ssDWorkUsageType ssSetDWorkUsageType(SimStruct* S, int_T vector, ssDWorkUsageType type);

#undef ssSetDWorkUsedAsDState
extern int_T ssSetDWorkUsedAsDState(SimStruct* S, int_T vector, int_T usage);

#undef ssSetDWorkWidth
extern int_T ssSetDWorkWidth(SimStruct* S, int_T vector, int_T width);

#undef ssGetNumDWork
extern int_T ssGetNumDWork(SimStruct* S);

#undef ssGetDWorkComplexSignal
extern CSignal_T ssGetDWorkComplexSignal(SimStruct* S, int_T vector);

#undef ssGetDWorkDataType
extern DTypeId ssGetDWorkDataType(SimStruct* S, int_T vector);

#undef ssGetDWorkName
extern const char_T*  ssGetDWorkName(SimStruct* S, int_T vector);

#undef ssGetDWorkUsageType
extern ssDWorkUsageType ssGetDWorkUsageType(SimStruct* S, int_T vector);

#undef ssGetDWorkUsedAsDState
extern int_T ssGetDWorkUsedAsDState(SimStruct* S, int_T vector);

#undef ssGetDWorkWidth
extern int_T ssGetDWorkWidth(SimStruct* S, int_T vector);

#undef ssGetDWork
extern void* ssGetDWork(SimStruct* S, int_T vector);

#undef ssGetdX
extern real_T* ssGetdX(SimStruct* S);

#undef ssGetNumContStates
extern int_T ssGetNumContStates(SimStruct* S);

#undef ssSetNumContStates
extern void ssSetNumContStates(SimStruct* S, int_T n);

#undef ssGetNumDiscStates
extern int_T ssGetNumDiscStates(SimStruct* S);

#undef ssSetNumDiscStates
extern void ssSetNumDiscStates(SimStruct* S, int_T n);

#undef ssSetNumNonsampledZCs
extern int_T ssSetNumNonsampledZCs(SimStruct* S, int_T n);

#undef ssGetNumNonsampledZCs
extern int_T ssGetNumNonsampledZCs(SimStruct* S);

#undef ssGetContStates
extern real_T* ssGetContStates(SimStruct* S);

#undef ssGetDiscStates
extern real_T* ssGetDiscStates(SimStruct* S);

#undef ssGetRealDiscStates
extern real_T* ssGetRealDiscStates(SimStruct* S);

#undef ssGetNonsampledZCs
extern real_T* ssGetNonsampledZCs(SimStruct* S);

#undef ssGetNumRWork
extern int_T ssGetNumRWork(SimStruct* S);

#undef ssSetNumRWork
extern void ssSetNumRWork(SimStruct* S, int_T n);

#undef ssGetRWork
extern real_T* ssGetRWork(SimStruct* S);

#undef ssGetRWorkValue
extern real_T ssGetRWorkValue(SimStruct* S, int_T idx);

#undef ssSetRWorkValue
extern real_T ssSetRWorkValue(SimStruct* S, int_T idx, real_T val);

#undef ssGetNumIWork
extern int_T ssGetNumIWork(SimStruct* S);

#undef ssSetNumIWork
extern void ssSetNumIWork(SimStruct* S, int_T n);

#undef ssGetIWork
extern int_T* ssGetIWork(SimStruct* S);

#undef ssGetIWorkValue
extern int_T ssGetIWorkValue(SimStruct* S, int_T idx);

#undef ssSetIWorkValue
extern int_T ssSetIWorkValue(SimStruct* S, int_T idx, int_T val);

#undef ssGetNumPWork
extern int_T ssGetNumPWork(SimStruct* S);

#undef ssSetNumPWork
extern int_T ssSetNumPWork(SimStruct* S, int_T n);

#undef ssGetPWork
extern void** ssGetPWork(SimStruct* S);

#undef ssGetPWorkValue
extern void* ssGetPWorkValue(SimStruct* S, int_T idx);

#undef ssSetPWorkValue
extern void* ssSetPWorkValue(SimStruct* S, int_T idx, void *pointer);

#undef ssGetNumModes
extern int_T ssGetNumModes(SimStruct* S);

#undef ssSetNumModes
extern int_T ssSetNumModes(SimStruct* S, int_T n);

#undef ssGetModeVector
extern int_T* ssGetModeVector(SimStruct* S);

#undef ssGetModeVectorValue
extern int_T ssGetModeVectorValue(SimStruct* S, int_T idx);

#undef ssSetModeVectorValue
extern void ssSetModeVectorValue(SimStruct* S, int_T idx, int_T val);

#undef ssGetErrorStatus
extern const char_T* ssGetErrorStatus(SimStruct* S);

#undef ssSetErrorStatus
extern void ssSetErrorStatus(SimStruct* S, const char_T* msg);

#undef ssGetOptions
extern uint_T ssGetOptions(SimStruct* S);

#undef ssSetOptions
extern void ssSetOptions(SimStruct* S, uint_T options);

#undef ssSetExternalModeFcn
extern void ssSetExternalModeFcn(SimStruct* S, SFunExtModeFcn fcn);

#undef ssGetModelName
extern const char_T*  ssGetModelName(SimStruct* S);

#undef ssGetParentSS
extern SimStruct* ssGetParentSS(SimStruct* S);

#undef ssGetPath
extern const char_T* ssGetPath(SimStruct* S);

#undef ssGetRootSS
extern SimStruct* ssGetRootSS(SimStruct* S);

#undef ssSetUserData
extern void ssSetUserData(SimStruct* S, void* data);

#undef ssSupportsMultipleExecInstances
extern void ssSupportsMultipleExecInstances(SimStruct* S, boolean_T val);

#undef ssGetSFcnLevel
extern int_T ssGetSFcnLevel(SimStruct* S);

#undef ssRTWGenIsCodeGen
extern boolean_T ssRTWGenIsCodeGen(SimStruct* S);

#undef ssSetPlacementGroup
extern void ssSetPlacementGroup(SimStruct* S, const char_T* groupName);

#undef ssGetPlacementGroup
extern const char_T* ssGetPlacementGroup(SimStruct* S);

#undef ssGetDWorkRTWIdentifier
extern char_T* ssGetDWorkRTWIdentifier(SimStruct* S, int_T idx);

#undef ssGetDWorkRTWIdentifierMustResolveToSignalObject
extern uint_T ssGetDWorkRTWIdentifierMustResolveToSignalObject(SimStruct* S, int_T idx);

#undef ssGetDWorkRTWTypeQualifier
extern char_T* ssGetDWorkRTWTypeQualifier(SimStruct* S, int_T idx);

#undef ssGetDWorkRTWStorageClass
extern int_T ssGetDWorkRTWStorageClass(SimStruct* S, int_T idx);

#undef ssSetDWorkRTWIdentifier
extern char_T* ssSetDWorkRTWIdentifier(SimStruct* S, int_T idx, char_T* val);

#undef ssSetDWorkRTWIdentifierMustResolveToSignalObject
extern uint_T ssSetDWorkRTWIdentifierMustResolveToSignalObject(SimStruct* S, int_T idx, uint_T val);

#undef ssSetDWorkRTWTypeQualifier
extern char_T* ssSetDWorkRTWTypeQualifier(SimStruct* S, int_T idx, char_T* val);

#undef ssSetDWorkRTWStorageClass
extern ssRTWStorageType ssSetDWorkRTWStorageClass(SimStruct* S, int_T idx, ssRTWStorageType val);

#undef ssRegMdlInfo
extern void ssRegMdlInfo(SimStruct* S, char* id, MdlInfoType infoTypeUsed, boolean_T b, int_T i, const void* vp);

#undef ssGetInputPortAcceptExprInRTW
extern boolean_T ssGetInputPortAcceptExprInRTW(SimStruct* S, int_T port);

#undef ssSetInputPortAcceptExprInRTW
extern void ssSetInputPortAcceptExprInRTW(SimStruct* S, int_T porot, boolean_T val);

#undef ssGetOutputPortConstOutputExprInRTW
extern boolean_T ssGetOutputPortConstOutputExprInRTW(SimStruct* S, int_T port);

#undef ssGetOutputPortTrivialOutputExprInRTW
extern boolean_T ssGetOutputPortTrivialOutputExprInRTW(SimStruct* S, int_T port);

#undef ssGetOutputPortOutputExprInRTW
extern boolean_T ssGetOutputPortOutputExprInRTW(SimStruct* S, int_T port);

#undef ssSetOutputPortConstOutputExprInRTW
extern void ssSetOutputPortConstOutputExprInRTW(SimStruct* S, int_T port, boolean_T val);

#undef ssSetOutputPortTrivialOutputExprInRTW
extern void ssSetOutputPortTrivialOutputExprInRTW(SimStruct* S, int_T port, boolean_T val);

#undef ssSetOutputPortOutputExprInRTW
extern void ssSetOutputPortOutputExprInRTW(SimStruct* S, int_T port, boolean_T val);

#ifdef mxGetNumberOfElements
#undef mxGetNumberOfElements
extern mwSize mxGetNumberOfElements(const mxArray *a);

#endif
#ifdef mxGetNumberOfDimensions
#undef mxGetNumberOfDimensions
extern mwSize mxGetNumberOfDimensions(const mxArray *a);

#endif
#ifdef mxGetN
#undef mxGetN
extern mwSize mxGetN(const mxArray *a);

#endif
#ifdef mxGetM
#undef mxGetM
extern mwSize mxGetM(const mxArray *a);

#endif
#ifdef mxGetScalar
#undef mxGetScalar
extern double mxGetScalar(const mxArray *a);

#endif
#ifdef mxGetData
#undef mxGetData
extern void *mxGetData(const mxArray *a);

#endif
#ifdef mxGetPr
#undef mxGetPr
extern double *mxGetPr(const mxArray *a);

#endif
#ifdef mxGetDimensions
#undef mxGetDimensions
extern mwSize *mxGetDimensions(const mxArray *a);

#endif
#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* __MW_WRAPPER_SIMSTRUC_H__*/
