class Sensor{
    private:
        char SEPARATOR = '|';
        byte motorAddress = 0;
        const byte GET_REAL_TIME = 0x0a, GET_HISTORY = 0X0B, CLEAR_HISTORY = 0X0A;
        void sendData(byte command, byte len);// send data to sensor
        void waitResponse();
        void splitResponse(String response, byte *data, byte *parameters, byte checkSum, byte parametersSize);
    public:
        Sensor(byte _motorAddress) : motorAddress(_motorAddress){
            Serial.begin(9600, SERIAL_8E1);
        };
        byte* getRealTime();
        byte* getHistory();
        byte* clearHistory();

};

byte* Sensor::getRealTime(){
    sendData(GET_REAL_TIME, 4);
    waitResponse();
    const byte parametersSize = 8;
    byte data[3], parameters[parametersSize], checkSum;
    splitResponse(Serial.readString(), data, parameters, &checkSum, parametersSize);
    return parameters;
}

byte* Sensor::getHistory(){
    sendData(GET_HISTORY, 4);
    waitResponse();
    const byte parametersSize = 11;
    byte data[3], parameters[parametersSize], checkSum;
    splitResponse(Serial.readString(), data, parameters, &checkSum, parametersSize);
    return parameters;
}

byte* Sensor::clearHistory(){
    sendData(CLEAR_HISTORY, 4);
    waitResponse();
    const byte parametersSize = 4;
    byte data[3], parameters[parametersSize], checkSum;
    splitResponse(Serial.readString(), data, parameters, &checkSum, parametersSize);
    return parameters;
}

void Sensor::sendData(byte command, byte len){
    String data = "";
    data += SEPARATOR;
    data += motorAddress;
    data += SEPARATOR;
    data += len + 3;
    data += SEPARATOR;
    data += command;
    data += SEPARATOR;
    byte opp = motorAddress + len + command;
    byte parameters[len] = {
        motorAddress,
        0X03,
        command,
        opp
    };
    for(byte i = 0; i < len; i++)
        data += parameters[i];
    data += SEPARATOR;
    data += opp;
    Serial.print(data);
}

void Sensor::waitResponse(){ // wait 2 seconds
    byte timeout = 200, counter = 0;
    while(!Serial.available() && counter < timeout){
        delay(10);
        counter++;
    }
}

void Sensor::splitResponse(String response, byte *data, byte *parameters, byte checkSum, byte parametersSize){
    response = response.substring(1); // delete first SEPARATOR
    //data[3]; //{dir, length, command}
    byte index = response.indexOf(SEPARATOR);
    for(byte i = 0; i < 3; i++){
        data[i] = (byte)response.substring(0, index).toInt();
        response = response.substring(index + 1);
        response.indexOf(SEPARATOR);
    }
    index = response.indexOf(SEPARATOR);
    String parametersBuffer = response.substring(0, index);
    parameters[parametersSize];
    for (byte i = 0; i < parametersSize; i++)
        parameters[i] = (byte)response.substring(i,i+1).toInt();
    response = response.substring(index+1);
    checkSum = (byte)response.toInt();
}