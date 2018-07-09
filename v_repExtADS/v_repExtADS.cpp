#include "v_repExtADS.h"
#include "scriptFunctionData.h"
#include "v_repLib.h"

#include "AdsLib/AdsLib.h"
#include <iomanip>
#include <iostream>
#ifdef _WIN32
#ifdef QT_COMPIL
#include <direct.h>
#else
#include <shlwapi.h>
#pragma comment(lib, "Shlwapi.lib")
#endif
#endif /* _WIN32 */
#if defined (__linux) || defined (__APPLE__)
#include <unistd.h>
#define WIN_AFX_MANAGE_STATE
#endif /* __linux || __APPLE__ */

#define CONCAT(x,y,z) x y z
#define strConCat(x,y,z)    CONCAT(x,y,z)

#define PLUGIN_NAME "ADS"

LIBRARY vrepLib;

////////////////////////////
long port=0; //ads port
AmsNetId remoteNetId;
AmsAddr server;//remote
uint32_t readHandle;
uint32_t writeHandle;

enum HANDLE_FLAGS
{
    HANDLE_CREATE=1,
    HANDLE_RELEASE=2,
};

void releaseHandle(uint32_t& handle)
{
    const long releaseHandle = AdsSyncWriteReqEx(port, &server, ADSIGRP_SYM_RELEASEHND, 0, sizeof(handle), &handle);
    if (releaseHandle) {
        std::cout << "ADS:Release handle 0x" << std::hex << handle << "' failed with: 0x" << releaseHandle << '\n';
    }
}

uint32_t getHandleByName(const std::string handleName)
{
    uint32_t handle = 0;
    const long handleStatus = AdsSyncReadWriteReqEx2(port,&server,
                                                     ADSIGRP_SYM_HNDBYNAME,
                                                     0,
                                                     sizeof(handle),
                                                     &handle,
                                                     handleName.size(),
                                                     handleName.c_str(),
                                                     nullptr);
    if (handleStatus) {
        std::cout << "ADS:Create handle for '" << handleName << "' failed with: 0x" << std::hex << handleStatus << '\n';
    }
    return handle;
}







// --------------------------------------------------------------------------------------
// simExtADS_create
// bool success=simADS.create(table_6 remoteNetId,string remoteIpV4,table_6 localNetId)
// --------------------------------------------------------------------------------------
#define LUA_CREATE_COMMAND "simADS.create"
const int inArgs_CREATE[]={
    3,
    sim_script_arg_int32|sim_script_arg_table,6,
    sim_script_arg_string,0,
    sim_script_arg_int32|sim_script_arg_table,6,
};
void LUA_CREATE_CALLBACK(SScriptCallBack* cb)
{
    CScriptFunctionData D;
    bool success=false;
    if (D.readDataFromStack(cb->stackID,inArgs_CREATE,inArgs_CREATE[0],LUA_CREATE_COMMAND))
    {
        //读取数据指针
        std::vector<CScriptFunctionDataItem>* inData=D.getInDataPtr();


        CScriptFunctionDataItem rids=inData->at(0);
        remoteNetId.b[0]=rids.int32Data[0];
        remoteNetId.b[1]=rids.int32Data[1];
        remoteNetId.b[2]=rids.int32Data[2];
        remoteNetId.b[3]=rids.int32Data[3];
        remoteNetId.b[4]=rids.int32Data[4];
        remoteNetId.b[5]=rids.int32Data[5];


        const char* remoteIpV4 = inData->at(1).stringData[0].c_str();

        CScriptFunctionDataItem lids=inData->at(2);
        //client的AmsNetId:需要在twincat添加该路由
        AdsSetLocalAddress({ (uint8_t)lids.int32Data[0],
                             (uint8_t)lids.int32Data[1],
                             (uint8_t)lids.int32Data[2],
                             (uint8_t)lids.int32Data[3],
                             (uint8_t)lids.int32Data[4],
                             (uint8_t)lids.int32Data[5] });

        server.port=AMSPORT_R0_PLC_TC3;
        if (!AdsAddRoute(remoteNetId, remoteIpV4))
        {
            server.netId=remoteNetId;
            success=true;
        }
        else
        {
            simSetLastError(LUA_CREATE_COMMAND,"Invalid Remote NetId.");
        }

    }
    D.pushOutData(CScriptFunctionDataItem(success));//返回值
    D.writeDataToStack(cb->stackID);
}
// --------------------------------------------------------------------------------------












// --------------------------------------------------------------------------------------
// simExtADS_read
// float[] result=simADS.read(string handleName,int floatLength,int handleFlag)
// --------------------------------------------------------------------------------------
#define LUA_READ_COMMAND "simADS.read"

const int inArgs_READ[]={
    3,
    sim_script_arg_string,0,
    sim_script_arg_int32,0,
    sim_script_arg_int32,0
};

void LUA_READ_CALLBACK(SScriptCallBack* cb)
{
    CScriptFunctionData D;


    if (D.readDataFromStack(cb->stackID,inArgs_READ,inArgs_READ[0],LUA_READ_COMMAND))
    {

        std::vector<CScriptFunctionDataItem>* inData=D.getInDataPtr();
        std::string handleName=inData->at(0).stringData[0];
        uint32_t floatLength=(uint32_t)inData->at(1).int32Data[0];//字节数
        std::vector<float> values(floatLength,0); //读取的数据
        uint32_t bytesRead;
        int flag=inData->at(2).int32Data[0];
        //----------更新readHandle---------------
        if(flag&HANDLE_CREATE)
        {
            readHandle=getHandleByName(handleName);
        }

        //---------------------------------------------------------------------------------
        const long status = AdsSyncReadReqEx2(port,&server,ADSIGRP_SYM_VALBYHND,readHandle,sizeof(float)*floatLength,&values[0],&bytesRead);


        if (!status)
        {
            D.pushOutData(CScriptFunctionDataItem(values));
        }
        else
        {
            D.pushOutData(CScriptFunctionDataItem());
            std::cout<<"ADS Read Data Failed"<<std::endl;
        }

        //---------释放readHandle-------------------------
        if(flag&HANDLE_RELEASE)
        {
            releaseHandle(readHandle);
        }

    }
    D.writeDataToStack(cb->stackID);


}
// --------------------------------------------------------------------------------------



// --------------------------------------------------------------------------------------
// simExtADS_write
//bool  success=simADS.write(string handleName,table floatData,int handleFlag)
// --------------------------------------------------------------------------------------
#define LUA_WRITE_COMMAND "simADS.write"

const int inArgs_WRITE[]={
    3,
    sim_script_arg_string,0,
    sim_script_arg_float|sim_script_arg_table,100,
    sim_script_arg_int32,0
};

void LUA_WRITE_CALLBACK(SScriptCallBack* cb)
{
    CScriptFunctionData D;


    if (D.readDataFromStack(cb->stackID,inArgs_WRITE,inArgs_WRITE[0],LUA_WRITE_COMMAND))
    {
        std::vector<CScriptFunctionDataItem>* inData=D.getInDataPtr();
        std::string handleName=inData->at(0).stringData[0];
        std::vector<float> values=inData->at(1).floatData;
        int flag=inData->at(2).int32Data[0];
        //----------更新readHandle---------------
        if(flag&HANDLE_CREATE)
        {
            writeHandle=getHandleByName(handleName);
        }

        //---------------------------------------------------------------------------------
        const long status = AdsSyncWriteReqEx(port, &server, ADSIGRP_SYM_VALBYHND, writeHandle, sizeof(float)*values.size(), &values[0]);

        if (!status)
        {
            D.pushOutData(CScriptFunctionDataItem(true));
        }
        else
        {
            D.pushOutData(CScriptFunctionDataItem(false));
            std::cout<<"ADS Write Data Failed"<<std::endl;
        }
        //---------释放readHandle-------------------------
        if(flag&HANDLE_RELEASE)
        {
            releaseHandle(writeHandle);
        }

    }
    D.writeDataToStack(cb->stackID);


}
// --------------------------------------------------------------------------------------














// --------------------------------------------------------------------------------------
// simExtADS_close:打开远程端口
// bool success=simADS.open()
// --------------------------------------------------------------------------------------
#define LUA_OPEN_COMMAND "simADS.open"
const int inArgs_OPEN[]={
    0
};

void LUA_OPEN_CALLBACK(SScriptCallBack* cb)
{
    CScriptFunctionData D;
    bool success=false;
    if (D.readDataFromStack(cb->stackID,inArgs_OPEN,inArgs_OPEN[0],LUA_OPEN_COMMAND))
    {
        port = AdsPortOpenEx();
        if (port)
        {
            success=true;
        }
        else
        {
            simSetLastError(LUA_OPEN_COMMAND,"Open ADS Port Failed.");
        }
    }
    D.pushOutData(CScriptFunctionDataItem(success));
    D.writeDataToStack(cb->stackID);
}
// --------------------------------------------------------------------------------------









// --------------------------------------------------------------------------------------
// simExtADS_close：关闭远程端口
// 	bool success=simADS.close()
// --------------------------------------------------------------------------------------
#define LUA_CLOSE_COMMAND "simADS.close"
const int inArgs_CLOSE[]={
    0
};

void LUA_CLOSE_CALLBACK(SScriptCallBack* cb)
{
    CScriptFunctionData D;
    bool success=false;
    if (D.readDataFromStack(cb->stackID,inArgs_CLOSE,inArgs_CLOSE[0],LUA_CLOSE_COMMAND))
    {
        const long closeStatus = AdsPortCloseEx(port);
        if (!closeStatus)
        {
            success=true;
        }
        else
        {
            simSetLastError(LUA_CLOSE_COMMAND,"Close ADS Port Failed.");
        }

    }
    D.pushOutData(CScriptFunctionDataItem(success));
    D.writeDataToStack(cb->stackID);
}
// --------------------------------------------------------------------------------------







VREP_DLLEXPORT unsigned char v_repStart(void* reservedPointer,int reservedInt)
{ // This is called just once, at the start of V-REP.
    // Dynamically load and bind V-REP functions:
    char curDirAndFile[1024];
#ifdef _WIN32
#ifdef QT_COMPIL
    _getcwd(curDirAndFile, sizeof(curDirAndFile));
#else
    GetModuleFileName(NULL,curDirAndFile,1023);
    PathRemoveFileSpec(curDirAndFile);
#endif
#elif defined (__linux) || defined (__APPLE__)
    getcwd(curDirAndFile, sizeof(curDirAndFile));
#endif

    std::string currentDirAndPath(curDirAndFile);
    std::string temp(currentDirAndPath);

#ifdef _WIN32
    temp+="\\v_rep.dll";
#elif defined (__linux)
    temp+="/libv_rep.so";
#elif defined (__APPLE__)
    temp+="/libv_rep.dylib";
#endif /* __linux || __APPLE__ */

    vrepLib=loadVrepLibrary(temp.c_str());
    if (vrepLib==NULL)
    {
        std::cout << "Error, could not find or correctly load v_rep.dll. Cannot start 'ADS' plugin.\n";
        return(0); // Means error, V-REP will unload this plugin
    }
    if (getVrepProcAddresses(vrepLib)==0)
    {
        std::cout << "Error, could not find all required functions in v_rep.dll. Cannot start 'ADS' plugin.\n";
        unloadVrepLibrary(vrepLib);
        return(0); // Means error, V-REP will unload this plugin
    }

    // Check the V-REP version:
    int vrepVer,vrepRev;
    simGetIntegerParameter(sim_intparam_program_version,&vrepVer);
    simGetIntegerParameter(sim_intparam_program_revision,&vrepRev);
    if( (vrepVer<30400) || ((vrepVer==30400)&&(vrepRev<9)) )
    {
        std::cout << "Sorry, your V-REP copy is somewhat old, V-REP 3.4.0 rev9 or higher is required. Cannot start 'ADS' plugin.\n";
        unloadVrepLibrary(vrepLib);
        return(0); // Means error, V-REP will unload this plugin
    }

    simRegisterScriptVariable("simADS","require('simExtADS')",0);

    // Register the new functions:
    simRegisterScriptCallbackFunction(strConCat(LUA_CREATE_COMMAND,"@",PLUGIN_NAME),strConCat("boolean success=",LUA_CREATE_COMMAND,"(table_6 remoteNetId,string remoteIpV4,table_6 localNetId)"),LUA_CREATE_CALLBACK);
    simRegisterScriptCallbackFunction(strConCat(LUA_OPEN_COMMAND,"@",PLUGIN_NAME),strConCat("boolean success=",LUA_OPEN_COMMAND,"()"),LUA_OPEN_CALLBACK);
    simRegisterScriptCallbackFunction(strConCat(LUA_CLOSE_COMMAND,"@",PLUGIN_NAME),strConCat("boolean success=",LUA_CLOSE_COMMAND,"()"),LUA_CLOSE_CALLBACK);
    simRegisterScriptCallbackFunction(strConCat(LUA_READ_COMMAND,"@",PLUGIN_NAME),strConCat("float[] floats=",LUA_READ_COMMAND,"(string handleName,int floatLength,int handleFlag)"),LUA_READ_CALLBACK);
    simRegisterScriptCallbackFunction(strConCat(LUA_WRITE_COMMAND,"@",PLUGIN_NAME),strConCat("boolean success=",LUA_WRITE_COMMAND,"(string handleName,table floatData,int handleFlag)"),LUA_WRITE_CALLBACK);

    return(9); // initialization went fine, we return the version number of this plugin (can be queried with simGetModuleName)
    // version 1 was for V-REP versions before V-REP 2.5.12
    // version 2 was for V-REP versions before V-REP 2.6.0
    // version 5 was for V-REP versions before V-REP 3.1.0
    // version 6 is for V-REP versions after V-REP 3.1.3
    // version 7 is for V-REP versions after V-REP 3.2.0 (completely rewritten)
    // version 8 is for V-REP versions after V-REP 3.3.0 (using stacks for data exchange with scripts)
    // version 9 is for V-REP versions after V-REP 3.4.0 (new API notation)
}

VREP_DLLEXPORT void v_repEnd()
{ // This is called just once, at the end of V-REP
    unloadVrepLibrary(vrepLib); // release the library
}

VREP_DLLEXPORT void* v_repMessage(int message,int* auxiliaryData,void* customData,int* replyData)
{ // This is called quite often. Just watch out for messages/events you want to handle
    // This function should not generate any error messages:
    int errorModeSaved;
    simGetIntegerParameter(sim_intparam_error_report_mode,&errorModeSaved);
    simSetIntegerParameter(sim_intparam_error_report_mode,sim_api_errormessage_ignore);

    void* retVal=NULL;
    if (message==sim_message_eventcallback_modulehandle)
    {
        //        if ( (customData==NULL)||(std::string("ADS").compare((char*)customData)==0) ) // is the command also meant for ADS?
        //        {

        //        }
    }

    // simulation ended. Destroy all ADS instances:
    if (message==sim_message_eventcallback_simulationended)
    {

    }

    simSetIntegerParameter(sim_intparam_error_report_mode,errorModeSaved); // restore previous settings
    return(retVal);
}
