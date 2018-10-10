#include "stdafx.h"

#include "Api.h"

namespace Contiva {
	namespace SAP {
		namespace NWRfc {
			namespace Native {

				Api::Api()
				{}

				ConnectionHandle^ Api::OpenConnection(IDictionary<String^, String^>^ connectionParams, RfcErrorInfo% errorInfo)
				{
					std::vector<RFC_CONNECTION_PARAMETER> connParams;
					RFC_ERROR_INFO errorInfoNative;
					marshal_context^ context = gcnew marshal_context();

					for each (KeyValuePair<String^, String^> kv in connectionParams)
					{
						RFC_CONNECTION_PARAMETER connParam;

						connParam.name = context->marshal_as<const wchar_t*>(kv.Key);
						connParam.value = context->marshal_as<const wchar_t*>(kv.Value);
						connParams.push_back(connParam);
					}

					RFC_CONNECTION_PARAMETER* connParamsArray = &connParams[0];
					RFC_CONNECTION_HANDLE handle = RfcOpenConnection(connParamsArray, connectionParams->Count, &errorInfoNative);
					errorInfo = NativeConverter::NativeRfcErrorInfoToManaged(errorInfoNative);

					return handle == NULL ? nullptr : gcnew ConnectionHandle(handle);


				}

				FunctionDescriptionHandle^ Api::GetFunctionDescription(ConnectionHandle^ connectionHandle, String^ functionName, RfcErrorInfo% errorInfo)
				{
					RFC_ERROR_INFO errorInfoNative;
					marshal_context^ context = gcnew marshal_context();

					RFC_FUNCTION_DESC_HANDLE handle = RfcGetFunctionDesc(connectionHandle->Handle, context->marshal_as<const SAP_UC*>(functionName), &errorInfoNative);
					errorInfo = NativeConverter::NativeRfcErrorInfoToManaged(errorInfoNative);
					return handle == NULL ? nullptr : gcnew FunctionDescriptionHandle(handle, false);

				}

				FunctionDescriptionHandle^ Api::GetFunctionDescription(FunctionHandle^ functionHandle, RfcErrorInfo% errorInfo)
				{
					RFC_ERROR_INFO errorInfoNative;

					RFC_FUNCTION_DESC_HANDLE handle = RfcDescribeFunction(functionHandle->Handle, &errorInfoNative);
					errorInfo = NativeConverter::NativeRfcErrorInfoToManaged(errorInfoNative);
					return handle == NULL ? nullptr : gcnew FunctionDescriptionHandle(handle, false);

				}

				RfcRc Api::GetFunctionName(FunctionDescriptionHandle^ descriptionHandle, String^% functionName, RfcErrorInfo% errorInfo)
				{
					RFC_ERROR_INFO errorInfoNative;
					RFC_ABAP_NAME nativeFunctionName;

					RFC_RC rc = RfcGetFunctionName(descriptionHandle->Handle, nativeFunctionName, &errorInfoNative);

					if (rc == RFC_RC::RFC_OK)
						functionName = marshal_as<String^>(nativeFunctionName);

					errorInfo = NativeConverter::NativeRfcErrorInfoToManaged(errorInfoNative);

					return (RfcRc)rc;

				}

				TypeDescriptionHandle^ Api::GetTypeDescription(IDataContainerHandle^ dataContainer, RfcErrorInfo% errorInfo)
				{
					RFC_ERROR_INFO errorInfoNative;

					RFC_TYPE_DESC_HANDLE handle = RfcDescribeType(dataContainer->DCHandle, &errorInfoNative);
					errorInfo = NativeConverter::NativeRfcErrorInfoToManaged(errorInfoNative);

					return handle == NULL ? nullptr : gcnew TypeDescriptionHandle(handle, false);

				}

				RfcRc Api::GetTypeFieldCount(TypeDescriptionHandle^ descriptionHandle, int% count, RfcErrorInfo% errorInfo)
				{
					RFC_ERROR_INFO errorInfoNative;
					unsigned int nativeCount;

					RFC_RC rc = RfcGetFieldCount(descriptionHandle->Handle, &nativeCount, &errorInfoNative);
					errorInfo = NativeConverter::NativeRfcErrorInfoToManaged(errorInfoNative);
					count = nativeCount;
					return (RfcRc)rc;

				}


				RfcRc Api::GetTypeFieldDescription(TypeDescriptionHandle^ descriptionHandle, int index, RfcFieldInfo^% fieldInfo, RfcErrorInfo% errorInfo)
				{
					RFC_ERROR_INFO errorInfoNative;

					RFC_FIELD_DESC nativeDescription;
					RFC_RC rc = RfcGetFieldDescByIndex(descriptionHandle->Handle, index, &nativeDescription, &errorInfoNative);

					errorInfo = NativeConverter::NativeRfcErrorInfoToManaged(errorInfoNative);
					fieldInfo = NativeConverter::NativeRfcFieldInfoToManaged(nativeDescription);
					return (RfcRc)rc;
				}


				RfcRc Api::GetTypeFieldDescription(TypeDescriptionHandle^ descriptionHandle, String^ name, RfcFieldInfo^% fieldInfo, RfcErrorInfo% errorInfo)
				{
					RFC_ERROR_INFO errorInfoNative;
					marshal_context^ context = gcnew marshal_context();

					RFC_FIELD_DESC nativeDescription;
					RFC_RC rc = RfcGetFieldDescByName(descriptionHandle->Handle, context->marshal_as< const RFC_CHAR*>(name), &nativeDescription, &errorInfoNative);

					errorInfo = NativeConverter::NativeRfcErrorInfoToManaged(errorInfoNative);
					fieldInfo = NativeConverter::NativeRfcFieldInfoToManaged(nativeDescription);

					return (RfcRc)rc;
				}

				FunctionHandle^ Api::CreateFunction(FunctionDescriptionHandle^ descriptionHandle, RfcErrorInfo% errorInfo)
				{
					RFC_ERROR_INFO errorInfoNative;

					RFC_FUNCTION_HANDLE handle = RfcCreateFunction(descriptionHandle->Handle, &errorInfoNative);
					errorInfo = NativeConverter::NativeRfcErrorInfoToManaged(errorInfoNative);
					return handle == NULL ? nullptr : gcnew FunctionHandle(handle);
				}


				RfcRc Api::GetFunctionParameterCount(FunctionDescriptionHandle^ descriptionHandle, int% count, RfcErrorInfo% errorInfo)
				{
					RFC_ERROR_INFO errorInfoNative;
					unsigned int nativeCount;

					RFC_RC rc = RfcGetParameterCount(descriptionHandle->Handle, &nativeCount, &errorInfoNative);
					errorInfo = NativeConverter::NativeRfcErrorInfoToManaged(errorInfoNative);
					count = nativeCount;
					return (RfcRc)rc;

				}


				RfcRc Api::GetFunctionParameterDescription(FunctionDescriptionHandle^ descriptionHandle, int index, RfcParameterInfo^% parameterInfo, RfcErrorInfo% errorInfo)
				{
					RFC_ERROR_INFO errorInfoNative;

					RFC_PARAMETER_DESC nativeDescription;
					RFC_RC rc = RfcGetParameterDescByIndex(descriptionHandle->Handle, index, &nativeDescription, &errorInfoNative);

					errorInfo = NativeConverter::NativeRfcErrorInfoToManaged(errorInfoNative);
					parameterInfo = NativeConverter::NativeRfcParameterInfoToManaged(nativeDescription);

					return (RfcRc)rc;

				}

				RfcRc Api::GetFunctionParameterDescription(FunctionDescriptionHandle^ descriptionHandle, String^ name, RfcParameterInfo^% parameterInfo, RfcErrorInfo% errorInfo)
				{
					RFC_ERROR_INFO errorInfoNative;
					marshal_context^ context = gcnew marshal_context();

					RFC_PARAMETER_DESC nativeDescription;
					RFC_RC rc = RfcGetParameterDescByName(descriptionHandle->Handle, context->marshal_as< const RFC_CHAR*>(name), &nativeDescription, &errorInfoNative);

					errorInfo = NativeConverter::NativeRfcErrorInfoToManaged(errorInfoNative);
					parameterInfo = NativeConverter::NativeRfcParameterInfoToManaged(nativeDescription);

					return (RfcRc)rc;

				}


				RfcRc Api::Invoke(ConnectionHandle^ connectionHandle, FunctionHandle^ functionHandle, RfcErrorInfo% errorInfo)
				{
					RFC_ERROR_INFO errorInfoNative;
					RFC_RC rc = RfcInvoke(connectionHandle->Handle, functionHandle->Handle, &errorInfoNative);

					errorInfo = NativeConverter::NativeRfcErrorInfoToManaged(errorInfoNative);
					return (RfcRc)rc;
				}

				RfcRc Api::GetStructure(IDataContainerHandle^ dataContainer, String^ name, StructureHandle^% structure, RfcErrorInfo% errorInfo)
				{
					RFC_ERROR_INFO errorInfoNative;
					marshal_context^ context = gcnew marshal_context();

					RFC_STRUCTURE_HANDLE createdHandle;
					RFC_RC rc = RfcGetStructure(dataContainer->DCHandle, context->marshal_as< const RFC_CHAR*>(name),
						&createdHandle, &errorInfoNative);

					errorInfo = NativeConverter::NativeRfcErrorInfoToManaged(errorInfoNative);

					if (rc == RFC_RC::RFC_OK)
						structure = gcnew StructureHandle(createdHandle, false);
					else
						structure = nullptr;

					return (RfcRc)rc;


				}

				RfcRc Api::GetTable(IDataContainerHandle^ dataContainer, String^ name, [Out] TableHandle^% table, [Out] RfcErrorInfo% errorInfo)
				{
					RFC_ERROR_INFO errorInfoNative;
					marshal_context^ context = gcnew marshal_context();

					RFC_TABLE_HANDLE createdHandle;
					RFC_RC rc = RfcGetTable(dataContainer->DCHandle, context->marshal_as< const RFC_CHAR*>(name),
						&createdHandle, &errorInfoNative);

					errorInfo = NativeConverter::NativeRfcErrorInfoToManaged(errorInfoNative);

					if (rc == RFC_RC::RFC_OK)
						table = gcnew TableHandle(createdHandle, false);
					else
						table = nullptr;

					return (RfcRc)rc;

				}

				TableHandle^ Api::CloneTable(TableHandle^ tableHandle, [Out] RfcErrorInfo% errorInfo)
				{
					RFC_ERROR_INFO errorInfoNative;
					marshal_context^ context = gcnew marshal_context();

					RFC_TABLE_HANDLE createdHandle;
					createdHandle = RfcCloneTable(tableHandle->Handle, &errorInfoNative);

					errorInfo = NativeConverter::NativeRfcErrorInfoToManaged(errorInfoNative);

					if (createdHandle != NULL)
						return gcnew TableHandle(createdHandle, false);
					else
						return nullptr;


				}

				RfcRc Api::GetTableRowCount(TableHandle^ table, [Out] int% count, [Out] RfcErrorInfo% errorInfo)
				{
					RFC_ERROR_INFO errorInfoNative;
					unsigned int nativeCount;

					RFC_RC rc = RfcGetRowCount(table->Handle, &nativeCount, &errorInfoNative);
					errorInfo = NativeConverter::NativeRfcErrorInfoToManaged(errorInfoNative);
					count = nativeCount;
					return (RfcRc)rc;

				}

				StructureHandle^ Api::GetCurrentTableRow(TableHandle^ table, [Out] RfcErrorInfo% errorInfo)
				{
					RFC_ERROR_INFO errorInfoNative;

					RFC_STRUCTURE_HANDLE createdHandle = RfcGetCurrentRow(table->Handle, &errorInfoNative);
					errorInfo = NativeConverter::NativeRfcErrorInfoToManaged(errorInfoNative);

					if (errorInfoNative.code == RFC_RC::RFC_OK && createdHandle != NULL)
						return gcnew StructureHandle(createdHandle, false);
					else
						return nullptr;

				}

				StructureHandle^ Api::AppendTableRow(TableHandle^ table, [Out] RfcErrorInfo% errorInfo)
				{
					RFC_ERROR_INFO errorInfoNative;

					RFC_STRUCTURE_HANDLE createdHandle = RfcAppendNewRow(table->Handle, &errorInfoNative);

					errorInfo = NativeConverter::NativeRfcErrorInfoToManaged(errorInfoNative);

					if (errorInfoNative.code == RFC_RC::RFC_OK && createdHandle != NULL)
						return gcnew StructureHandle(createdHandle, false);
					else
						return nullptr;
				}

				RfcRc Api::MoveToNextTableRow(TableHandle^ table, [Out] RfcErrorInfo% errorInfo)
				{
					RFC_ERROR_INFO errorInfoNative;

					RFC_RC rc = RfcMoveToNextRow(table->Handle, &errorInfoNative);

					errorInfo = NativeConverter::NativeRfcErrorInfoToManaged(errorInfoNative);
					return (RfcRc)rc;

				}

				RfcRc Api::MoveToFirstTableRow(TableHandle^ table, [Out] RfcErrorInfo% errorInfo)
				{
					RFC_ERROR_INFO errorInfoNative;

					RFC_RC rc = RfcMoveToFirstRow(table->Handle, &errorInfoNative);

					errorInfo = NativeConverter::NativeRfcErrorInfoToManaged(errorInfoNative);
					return (RfcRc)rc;

				}


				RfcRc Api::SetString(IDataContainerHandle^ containerHandle, String^ name, String^ value, RfcErrorInfo% errorInfo)
				{
					RFC_ERROR_INFO errorInfoNative;
					marshal_context^ context = gcnew marshal_context();

					RFC_RC rc = RfcSetChars(containerHandle->DCHandle,
						context->marshal_as<const SAP_UC*>(name),
						context->marshal_as< const RFC_CHAR*>(value), value->Length, &errorInfoNative);

					errorInfo = NativeConverter::NativeRfcErrorInfoToManaged(errorInfoNative);
					return (RfcRc)rc;

				}

				RfcRc Api::GetString(IDataContainerHandle^ containerHandle, String^ name, String^% value, RfcErrorInfo% errorInfo)
				{
					RFC_ERROR_INFO errorInfoNative;
					marshal_context^ context = gcnew marshal_context();

					SAP_UC valueBuffer[61] = iU("");
					unsigned resultLen;

					RFC_RC rc = RfcGetString(containerHandle->DCHandle,
						context->marshal_as<const SAP_UC*>(name), valueBuffer, 61, &resultLen, &errorInfoNative);

					if (rc == RFC_RC::RFC_BUFFER_TOO_SMALL)
					{
						SAP_UC* dynamicBuffer = new SAP_UC[resultLen + 1];

						try {
							rc = RfcGetString(containerHandle->DCHandle,
								context->marshal_as<const SAP_UC*>(name), dynamicBuffer, resultLen + 1, &resultLen, &errorInfoNative);

							value = gcnew String(dynamicBuffer);
						}
						finally{
							delete dynamicBuffer;
						}

					}
					else
					{
						value = gcnew String(valueBuffer);
					}


					errorInfo = NativeConverter::NativeRfcErrorInfoToManaged(errorInfoNative);
					return (RfcRc)rc;


				}

				void Api::AllowStartOfPrograms(ConnectionHandle^ connectionHandle, StartProgramDelegate^ callback, RfcErrorInfo% errorInfo) {

					RFC_ERROR_INFO nativeErrorInfo;

					RFC_FUNCTION_DESC_HANDLE RFC_START_PROGRAM_Desc = RfcCreateFunctionDesc(cU("RFC_START_PROGRAM"), &nativeErrorInfo);
					if (RFC_START_PROGRAM_Desc == NULL)
					{
						errorInfo = NativeConverter::NativeRfcErrorInfoToManaged(nativeErrorInfo);
						return;
					}

					RFC_PARAMETER_DESC paramDesc = { iU("COMMAND"), RFCTYPE_CHAR, RFC_IMPORT, 512, 1024, 0, 0, 0, 0, 0 };
					RFC_RC rc = RfcAddParameter(RFC_START_PROGRAM_Desc, &paramDesc, &nativeErrorInfo);
					if (rc != RFC_RC::RFC_OK)
					{
						errorInfo = NativeConverter::NativeRfcErrorInfoToManaged(nativeErrorInfo);
						return;
					}

					rc = RfcInstallServerFunction(NULL, RFC_START_PROGRAM_Desc, RFC_START_PROGRAM_Handler, &nativeErrorInfo);
					if (rc != RFC_RC::RFC_OK)
					{
						errorInfo = NativeConverter::NativeRfcErrorInfoToManaged(nativeErrorInfo);
						return;
					}

					StartProgramCallback = callback;
				}

				RfcErrorInfo Api::StartProgramHandler(String^ command)
				{
					if (StartProgramCallback != nullptr)
						return StartProgramCallback(command);

					return RfcErrorInfo(RfcRc::RFC_EXTERNAL_FAILURE, RfcErrorGroup::EXTERNAL_RUNTIME_FAILURE, "",
						"Start of external programs no longer enabled", "", "", "", "", "", "", "");
				}


				RFC_RC SAP_API RFC_START_PROGRAM_Handler(RFC_CONNECTION_HANDLE rfcHandle, RFC_FUNCTION_HANDLE funcHandle, RFC_ERROR_INFO* errorInfoP)
				{
					SAP_UC command[513] = iU("");
					unsigned length;

					RfcGetStringByIndex(funcHandle, 0, command, sizeofU(command) - 1, &length, errorInfoP);
					RfcErrorInfo managedErrorInfo = Api::StartProgramHandler(gcnew String(command));

					NativeConverter::ManagedRfcErrorInfoToNative(managedErrorInfo, errorInfoP);
					return errorInfoP->code;
				}

			}
		}
	}
}

