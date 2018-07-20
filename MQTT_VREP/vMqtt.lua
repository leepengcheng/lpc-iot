
function sysCall_init()
    local MQTT = require "mqtt"
    local socket = require "socket"
     args={
      host="127.0.0.1",
      port=1883,
      id="client1",
      topic_s="#",
      topic_p="/test2",
      debug=true
    }
    mqtt_client = MQTT.client.create(args.host, args.port, callback)
    MQTT.Utility.set_debug(false)
    error_message = mqtt_client:connect(args.id)
    if error_message ~= nil then 
        error(error_message) 
    end
    mqtt_client:subscribe({ args.topic_s })
end
function callback(topic,message)
  print("Topic: " .. topic .. ", message: '" .. message .. "'")
  -- mqtt_client:publish(args.topic_p, message)
end

function sysCall_actuation()
    if error_message == nil then
      error_message = mqtt_client:handler()
      --socket.sleep(1.0)  -- seconds
    else
      print("Error:"..error_message)
    end
end

function sysCall_sensing()
    -- put your sensing code here
end

function sysCall_cleanup()
      mqtt_client:unsubscribe({ args.topic_s })
      mqtt_client:destroy()
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
