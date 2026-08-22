cpu = esp8684 (2MB flash, DIO, 40 MHz XTAL)

Stock Arduino-ESP32 C2 bootloader/libs are 26 MHz. This project uses
`framework = arduino, espidf` so bootloader and app share CONFIG_XTAL_FREQ=40.

# UDP echo server
- WiFi STA + WiFiUDP, listen on UDP_PORT (default 5000)
- ztask poll: receive datagram -> Serial log -> echo back to peer
- UART CLI: `ip rd` shows IP / gateway / RSSI / udp port

# Test (PC same LAN)
```
# Linux / Git Bash
echo -n hello | nc -u -w1 <ESP_IP> 5000

# PowerShell
$udp = New-Object System.Net.Sockets.UdpClient
$udp.Connect("<ESP_IP>", 5000)
$bytes = [Text.Encoding]::ASCII.GetBytes("hello")
$udp.Send($bytes, $bytes.Length) | Out-Null
$remote = New-Object System.Net.IPEndPoint([Net.IPAddress]::Any, 0)
[Text.Encoding]::ASCII.GetString($udp.Receive([ref]$remote))
$udp.Close()
```
