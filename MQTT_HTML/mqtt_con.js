var hostname = '127.0.0.1';
var port = 9001;
var clientId = 'lee19870817';
var timeout = 5;
var keepAlive = 100;
var cleanSession = false;
var ssl = false;
var userName = 'lee';
var password = '10086';
var topic = '/path';

//建立客户端实例 
client = new Paho.MQTT.Client(hostname, port, clientId);


var options = {
    invocationContext: {
        host: hostname,
        port: port,
        path: client.path,
        clientId: clientId
    },
    timeout: timeout,
    keepAliveInterval: keepAlive,
    cleanSession: cleanSession,
    useSSL: ssl,
    // userName: userName,  
    // password: password,  
    onSuccess: onConnect,
    onFailure: onFailure
};
client.connect(options);


//连接服务器并注册连接石板处理事件  
function onFailure(e) {
    console.log(e);
    s = "连接失败";
}
//连接服务器并注册连接成功处理事件  
function onConnect() {
    console.log("onConnected");
    console.log("连接成功");
    // client.subscribe(topic);
}

client.onConnectionLost = onConnectionLost;
// client.onMessageArrived = onMessageArrived;


//注册连接断开处理事件  
function onConnectionLost(responseObject) {
    console.log(responseObject);
    if (responseObject.errorCode !== 0) {
        console.log("onConnectionLost:" + responseObject.errorMessage);
        console.log("连接已断开");
    }
}

//注册消息接收处理事件  
// function onMessageArrived(message) {
//     s = "{time:" + new Date().Format("yyyy-MM-dd hh:mm:ss") + ", onMessageArrived()}";
//     console.log(s);
//     console.log("收到消息:" + message.payloadString);
// }

//发送消息
function send() {
    if (true) {
        var nodes = myDiagram.model.nodeDataArray
        message = new Paho.MQTT.Message(nodes.toJson());
        message.destinationName = topic;
        client.send(message);
    }
}