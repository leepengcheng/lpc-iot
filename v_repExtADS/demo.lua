
--将vrep发送的uint32 table转为float table
function vrep_Int_Float(uint32NumTable)
    local numBufferString=sim.packUInt32Table(uint32NumTable)
    local floatNumTable=sim.unpackFloatTable(numBufferString)
    return floatNumTable
end


--将从twincat接收到的float table转换为uint32 table
function twincat_Uint32_Float(floatNumTable)
    local numBufferString=sim.packFloatTable(floatNumTable)
    local uint32NumTable=sim.unpackUInt32Table(numBufferString)
    return uint32NumTable
end


function sysCall_init()
    gHandle=sim.getObjectHandle("Graph")
    READ_ADDR="MAIN.NUM"
    WRITE_ADDR="MAIN.NUM"
    simADS.create({192,168,6,90,1,1},"127.0.0.1",{192,168,6,90,2,1})
    simADS.read(READ_ADDR,0,simADS_handle_open)    --open read Handle
    simADS.write(WRITE_ADDR,{},simADS_handle_open) --open write handle 
    num=123456
end

function sysCall_actuation()
    ------0:write with out open/close handle
    num=num+1
    local sendTable=vrep_Int_Float({num})
    simADS.write(WRITE_ADDR,sendTable,simADS_handle_none)

    ------0:read with out open/close handle
    local dataTable=simADS.read(READ_ADDR,1,simADS_handle_none)
    local revTable=twincat_Uint32_Float(dataTable)
    
    if num~=revTable[1] then
        print("error")
    else
        print(num)
    end
    --sim.setGraphUserData(gHandle,"data",r[1])
    --print(string.format("Read  data:%s %s %s",r[1],r[2],r[3]))
    --print("***************************************************")
end

function sysCall_sensing()
    -- put your sensing code here
end

function sysCall_cleanup()
    simADS.read(READ_ADDR,0,simADS_handle_close)     --close read Handle
    simADS.write(WRITE_ADDR,{},simADS_handle_close)  --close write Handle
    simADS.destory()
end

-- You can define additional system calls here:
--[[
function sysCall_suspend()
end

function sysCall_resume()
end

function sysCall_dynCallback(inData)
end

function sysCall_jointCallback(inData)
    return outData
end

function sysCall_contactCallback(inData)
    return outData
end

function sysCall_beforeCopy(inData)
    for key,value in pairs(inData.objectHandles) do
        print("Object with handle "..key.." will be copied")
    end
end

function sysCall_afterCopy(inData)
    for key,value in pairs(inData.objectHandles) do
        print("Object with handle "..key.." was copied")
    end
end

function sysCall_beforeDelete(inData)
    for key,value in pairs(inData.objectHandles) do
        print("Object with handle "..key.." will be deleted")
    end
    -- inData.allObjects indicates if all objects in the scene will be deleted
end

function sysCall_afterDelete(inData)
    for key,value in pairs(inData.objectHandles) do
        print("Object with handle "..key.." was deleted")
    end
    -- inData.allObjects indicates if all objects in the scene were deleted
end
--]]
