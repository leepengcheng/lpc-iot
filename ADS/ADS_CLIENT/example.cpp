
#include "AdsLib.h"

#include <iostream>
#include <iomanip>
const uint8_t TRAJ_CMD_STOP= 0;//停止
const uint8_t TRAJ_CMD_NEW= 1;//读入轨迹
const uint8_t TRAJ_CMD_START= 2;//开始运动

struct TRAJ_POINT
{
	uint32_t timestamp;				//时间戳(ms)
	float	joints[7];				//关节的位置数据(暂定7关节)
};
struct TRAJECTORY
{
	uint16_t traj_size;					//点的个数
	TRAJ_POINT	points[100];				//关节的位置数据(暂定7关节)
};

static void NotifyCallback(const AmsAddr* pAddr, const AdsNotificationHeader* pNotification, uint32_t hUser)
{
    const uint8_t* data = reinterpret_cast<const uint8_t*>(pNotification + 1);
    std::cout << std::setfill('0') <<
        "NetId: " << pAddr->netId <<
        " hUser 0x" << std::hex << hUser <<
        " sample time: " << std::dec << pNotification->nTimeStamp <<
        " sample size: " << std::dec << pNotification->cbSampleSize <<
        " value:";
	//<< std::hex 
    for (size_t i = 0; i < pNotification->cbSampleSize; ++i) {
        std::cout << " 0x" << (int)data[i];
    }
    std::cout << '\n';
}

uint32_t getHandleByNameExample(std::ostream& out, long port, const AmsAddr& server,
                                const std::string handleName)
{
    uint32_t handle = 0;
    const long handleStatus = AdsSyncReadWriteReqEx2(port,
                                                     &server,
                                                     ADSIGRP_SYM_HNDBYNAME,
                                                     0,
                                                     sizeof(handle),
                                                     &handle,
                                                     handleName.size(),
                                                     handleName.c_str(),
                                                     nullptr);
    if (handleStatus) {
        out << "Create handle for '" << handleName << "' failed with: 0x" << std::hex << handleStatus << '\n';
    }
    return handle;
}

void releaseHandleExample(std::ostream& out, long port, const AmsAddr& server, uint32_t handle)
{
    const long releaseHandle = AdsSyncWriteReqEx(port, &server, ADSIGRP_SYM_RELEASEHND, 0, sizeof(handle), &handle);
    if (releaseHandle) {
        out << "Release handle 0x" << std::hex << handle << "' failed with: 0x" << releaseHandle << '\n';
    }
}

void notificationExample(std::ostream& out, long port, const AmsAddr& server)
{
    const AdsNotificationAttrib attrib = {
        1,
        ADSTRANS_SERVERONCHA,
        0,
        {4000000}
    };
    uint32_t hNotify;
    uint32_t hUser = 0;

    const long addStatus = AdsSyncAddDeviceNotificationReqEx(port,
                                                             &server,
                                                             0x4020,
                                                             0,
                                                             &attrib,
                                                             &NotifyCallback,
                                                             hUser,
                                                             &hNotify);
    if (addStatus) {
        out << "Add device notification failed with: " << std::dec << addStatus << '\n';
        return;
    }

    std::cout << "Hit ENTER to stop notifications\n";
    std::cin.ignore();

    const long delStatus = AdsSyncDelDeviceNotificationReqEx(port, &server, hNotify);
    if (delStatus) {
        out << "Delete device notification failed with: " << std::dec << delStatus;
        return;
    }
}

void notificationByNameExample(std::ostream& out, long port, const AmsAddr& server)
{
    const AdsNotificationAttrib attrib = {
        1,
        ADSTRANS_SERVERCYCLE,
        0,
        {4000000}
    };
    uint32_t hNotify;
    uint32_t hUser = 0;

    uint32_t handle;

    out << __FUNCTION__ << "():\n";
    handle = getHandleByNameExample(out, port, server, "MAIN.byByte[4]");
	
    const long addStatus = AdsSyncAddDeviceNotificationReqEx(port,
                                                             &server,
                                                             ADSIGRP_SYM_VALBYHND,
                                                             handle,
                                                             &attrib,
                                                             &NotifyCallback,
                                                             hUser,
                                                             &hNotify);
    if (addStatus) {
        out << "Add device notification failed with: " << std::dec << addStatus << '\n';
        return;
    }

    std::cout << "Hit ENTER to stop by name notifications\n";
    std::cin.ignore();

    const long delStatus = AdsSyncDelDeviceNotificationReqEx(port, &server, hNotify);
    if (delStatus) {
        out << "Delete device notification failed with: " << std::dec << delStatus;
        return;
    }
    releaseHandleExample(out, port, server, handle);
}

void readExample(std::ostream& out, long port, const AmsAddr& server)
{
    uint32_t bytesRead;
    uint32_t buffer;

    out << __FUNCTION__ << "():\n";
    for (size_t i = 0; i < 8; ++i) {
        const long status = AdsSyncReadReqEx2(port, &server, 0x4020, 0, sizeof(buffer), &buffer, &bytesRead);
        if (status) {
            out << "ADS read failed with: " << std::dec << status << '\n';
            return;
        }
        out << "ADS read " << std::dec << bytesRead << " bytes, value: 0x" << std::hex << buffer << '\n';
    }
}
void writeExample(std::ostream& out, long port, const AmsAddr& server)
{
	uint8_t buffer_write[10] = {7,4,7,9,5,4,3,2,1,8};
	uint32_t writeLength = sizeof(uint8_t) * 10;
	out << __FUNCTION__ << "():\n";
	const long status=AdsSyncWriteReqEx(port, &server, 0x4020, 0, writeLength, buffer_write);
}

//写入值
void writeJointValueByName(std::ostream& out, long port, const AmsAddr& server, QString input)
{
	//获得变量的句柄
	uint32_t handle1, handle2;
	handle1 = getHandleByNameExample(out, port, server, "TRAJ.trajectory");
	handle2 = getHandleByNameExample(out, port, server, "MAIN.traj_cmd");


	//轨迹信息
	TRAJECTORY trajectory;
	trajectory.traj_size = 0;
	uint8_t traj_cmd;
	//关节数据
	for (int i = 0; i < 100; ++i)
	{
		trajectory.points[i].timestamp = i;
		for (int j = 0; j < 7; ++j)
		{
			trajectory.points[i].joints[j] = static_cast<float>(i + j);
		}
	}

	
	if (input=="START")
	{
		std::cout << "START" << std::endl;
		traj_cmd = TRAJ_CMD_START;
		const long status2 = AdsSyncWriteReqEx(port, &server, ADSIGRP_SYM_VALBYHND, handle2, sizeof(uint8_t), &traj_cmd);
	} 
	else if(input == "STOP")
	{
		std::cout << "STOP" << std::endl;
		traj_cmd = TRAJ_CMD_STOP;
		//发送执行命令
		const long status2 = AdsSyncWriteReqEx(port, &server, ADSIGRP_SYM_VALBYHND, handle2, sizeof(uint8_t), &traj_cmd);
	}
	else
	{
		std::cout << "NEW" << std::endl;
		traj_cmd = TRAJ_CMD_NEW;
		trajectory.traj_size = 100;
		//发送轨迹信息sizeof(TRAJECTORY)
		const long status1 = AdsSyncWriteReqEx(port, &server, ADSIGRP_SYM_VALBYHND, handle1, sizeof(TRAJECTORY), &trajectory);
		//发送执行命令
		const long status2 = AdsSyncWriteReqEx(port, &server, ADSIGRP_SYM_VALBYHND, handle2, sizeof(uint8_t), &traj_cmd);
	}
	
	releaseHandleExample(out, port, server, handle1);
	releaseHandleExample(out, port, server, handle2);


}

void readJointValueByName(std::ostream& out, long port, const AmsAddr& server)
{
	uint32_t bytesRead;
	float buffer[8];
	uint32_t handle;

	out << __FUNCTION__ << "():\n";
	handle = getHandleByNameExample(out, port, server, "TRAJ.trajPoints[0].joints");
	out << "joints Handle " << handle << '\n';
	const long status = AdsSyncReadReqEx2(port,
			&server,
			ADSIGRP_SYM_VALBYHND,
			handle,
			sizeof(buffer)*8,
			&buffer,
			&bytesRead);
	if (status) {
		out << "ADS read failed with: " << std::dec << status << '\n';
		return;
	}
	releaseHandleExample(out, port, server, handle);
}
void writeAndReadJointValueByName(std::ostream& out, long port, const AmsAddr& server)
{
	uint32_t bytesRead;
	float buffer[7];
	uint32_t handle;

	handle = getHandleByNameExample(out, port, server, "TRAJ.trajPoints");
	out << "joints Handle " << handle << '\n';
	const long status = AdsSyncReadReqEx2(port,
		&server,
		ADSIGRP_SYM_VALBYHND,
		handle,
		sizeof(buffer) * 7,
		&buffer,
		&bytesRead);
	if (status) {
		out << "ADS read failed with: " << std::dec << status << '\n';
		return;
	}
	releaseHandleExample(out, port, server, handle);
}




void readByNameExample(std::ostream& out, long port, const AmsAddr& server)
{
    uint32_t bytesRead;
	uint8_t buffer;
    uint32_t handle;

    out << __FUNCTION__<< "():\n";
    //handle = getHandleByNameExample(out, port, server, "MAIN.byByte[4]");
	handle = getHandleByNameExample(out, port, server, "TRAJ.trajPoints[0].joints[0]");

    for (size_t i = 0; i < 8; ++i) {
        const long status = AdsSyncReadReqEx2(port,
                                              &server,
                                              ADSIGRP_SYM_VALBYHND,
                                              handle,
                                              sizeof(buffer),
                                              &buffer,
                                              &bytesRead);
        if (status) {
            out << "ADS read failed with: " << std::dec << status << '\n';
            return;
        }


        out << "ADS read " << std::dec << bytesRead << " bytes, value: 0x" << std::hex << (int)buffer << '\n';
    }
    releaseHandleExample(out, port, server, handle);
}



void readStateExample(std::ostream& out, long port, const AmsAddr& server)
{
    uint16_t adsState;
    uint16_t devState;

    const long status = AdsSyncReadStateReqEx(port, &server, &adsState, &devState);
    if (status) {
        out << "ADS read failed with: " << std::dec << status << '\n';
        return;
    }
    out << "ADS state: " << std::dec << adsState << " devState: " << std::dec << devState << '\n';
}

void runExample(std::ostream& out,QString input)
{
    static const AmsNetId remoteNetId { 192,168,6,90,1,1 };
    static const char remoteIpV4[] = "127.0.0.1";

	AdsSetLocalAddress({ 192,168,6,90,3,1 });

    // add local route to your EtherCAT Master
    if (AdsAddRoute(remoteNetId, remoteIpV4)) {
        out << "Adding ADS route failed, did you specified valid addresses?\n";
        return;
    }

    // open a new ADS port
    const long port = AdsPortOpenEx();
    if (!port) {
        out << "Open ADS port failed\n";
        return;
    }

    const AmsAddr remote { remoteNetId, AMSPORT_R0_PLC_TC3 };

	//writeExample(out, port, remote);
	writeJointValueByName(out, port, remote,input);
	//readJointValueByName(out, port, remote);
    //notificationExample(out, port, remote);
    //notificationByNameExample(out, port, remote);
	//readExample(out, port, remote);
    ///readByNameExample(out, port, remote);
    //readStateExample(out, port, remote);




    const long closeStatus = AdsPortCloseEx(port);
    if (closeStatus) {
        out << "Close ADS port failed with: " << std::dec << closeStatus << '\n';
    }

#ifdef _WIN32
    // WORKAROUND: On Win7 std::thread::join() called in destructors
    //             of static objects might wait forever...
    AdsDelRoute(remoteNetId);
#endif
}

int main()
{
	char input[24];
	std::cout << "input EXIT | START |STOP |NEW \n";
	while (true)
	{
		std::cin.getline(input, 10);
		QString data(input);
		data = data.trimmed();
		if (data=="EXIT" || data=="exit")
		{
			break;
		}
		runExample(std::cout, data);
	}

}
