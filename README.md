# Packet-Structure

### Data - Sample Format
| TITLE     | SIZE  | START | END | SAMPLE                  |
|-----------|-------|-------|-----|-------------------------|
| HEADER    | 4     | 0     | 3   | AFDBAA00                |
| COUNTER   | 4     | 4     | 7   | 00000001                |
| TIMESTAMP | 8     | 8     | 15  | 00000001                |
| UID       | 16    | 16    | 31  | 0000000000000001        |
| PAYLOAD   | 2*128 | 32    | 287 | X (16bit value = 2Byte * 128 data) |
| RESERVED  | 8     | 288   | 295 | 0x00                    |
| FOOTER    | 4     | 296   | 299 | FFAADF00                |


``` cpp
void UpdateAndSendData()
{
	// Header
	DataBuffer[0] = 0xAF;
	DataBuffer[1] = 0xDB;
	DataBuffer[2] = 0xAA;
	DataBuffer[3] = 0x00;
	// Footer
	DataBuffer[CLIENT_DATA_BUFFER_SIZE - 4] = 0xFF;
	DataBuffer[CLIENT_DATA_BUFFER_SIZE - 3] = 0xAA;
	DataBuffer[CLIENT_DATA_BUFFER_SIZE - 2] = 0xDF;
	DataBuffer[CLIENT_DATA_BUFFER_SIZE - 1] = 0x00;

	// Counter
	DataBuffer[4] = Data_SendCounter >> 24;
	DataBuffer[5] = Data_SendCounter >> 16;
	DataBuffer[6] = Data_SendCounter >> 8;
	DataBuffer[7] = Data_SendCounter >> 0;
	Data_SendCounter++;

	...

	// SEND...
	Client_UpdateData(DataBuffer, CLIENT_DATA_BUFFER_SIZE);
}
```
