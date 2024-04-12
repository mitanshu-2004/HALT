# H.A.L.T 
The project had multiple (Proximity sensors embedded on the road to determine the density of traffic waiting on a particular signal and accordingly control the traffic lights. Apart from this the red-light sensors also had a RFID receiver which detected a passing emergency vehicle such as an ambulance and appropriately changed the traffic lights.In addition to this grafana was used to represent data in a dynamic  way.
(https://user-images.githubusercontent.com/84952780/170506027-3b1c18b3-90db-4641-b290-73dca9440ade.png)
 ![Holistic traffic management system - Presentation - Google Chrome 11-06-2022 22_17_29](https://user-images.githubusercontent.com/84952780/173198548-de8e2d2e-53cd-4c11-8c58-bd520c489844.png)
 
  ### Tech Stack used

  This is an IoT enabled Traffic Management System.
  In this project we have used multiple Tech. stacks like:
  - Django
  - Grafana
  - Fusion 360
  - Prometheus
  - C++
  - Python
  - Arduino IDE

  Furthermore we have used:
  - RFID Sensor
  - Servo
  - IR sensor
  - LCD 
  
  
  We even used Open cv for better detection of traffic.  

## Requirements

- Grafana
- Prometheus
- Redis
- Fast-API
- Arduino IDE
- Uvicorn

## Installation

- ### Installing python dependencies

```bash
pip3 install -t requirements.txt
```

- ### Installing redis-server

```bash
sudo apt install redis-server
```

- ### Installing Grafana

```bash
sudo apt-get install -y apt-transport-https
sudo apt-get install -y software-properties-common wget
wget -q -O - https://packages.grafana.com/gpg.key | sudo apt-key add -
echo "deb https://packages.grafana.com/oss/deb stable main" | sudo tee -a /etc/apt/sources.list.d/grafana.list
sudo apt-get update
sudo apt-get install grafana
```

Starting the Grafana-server

```bash
sudo systemctl daemon-reload
sudo systemctl start grafana-server
sudo systemctl status grafana-server
sudo systemctl enable grafana-server.service
```
(last line configures it to run at boot)

Now you can head over to http://localhost:3000/login (the default username and password is '__admin__')

- ### Installing Prometheus
```bash
sudo apt update && sudo apt install wget -y
sudo useradd --system --no-create-home --shell /usr/sbin/nologin prometheus
```
```bash
cd ~/downloads
wget https://github.com/prometheus/prometheus/releases/download/v2.28.0/prometheus-2.28.0.linux-amd64.tar.gz
tar xvzf prometheus-2.28.0.linux-amd64.tar.gz
sudo mv -v prometheus-2.28.0.linux-amd64 /opt/prometheus
sudo chown -Rfv root:root /opt/prometheus
sudo chmod -Rfv 0755 /opt/prometheus
sudo mkdir -v /opt/prometheus/data
sudo chown -Rfv prometheus:prometheus /opt/prometheus/data
sudo nano /etc/systemd/system/prometheus.service
```

Now paste the code given below.

```text
[Unit]
Description=Monitoring system and time series database

[Service]
Restart=always
User=prometheus
ExecStart=/opt/prometheus/prometheus --config.file=/opt/prometheus/prometheus.yml --storage.tsdb.path=/opt/prometheus/data
ExecReload=/bin/kill -HUP $MAINPID
TimeoutStopSec=20s
SendSIGKILL=no
LimitNOFILE=8192

[Install]
WantedBy=multi-user.target
```
Press ctrl+o and then press the Enter key, followed by ctrl+x.

Starting the Prometheus-server

```bash
sudo systemctl daemon-reload
sudo systemctl start prometheus.service
sudo systemctl enable prometheus.service
```
(last line configures it to run at boot).

Prometheus is now Installed, navigate to the URL http://localhost:9090/targets from your favorite web browser and all the targets that you’ve configured should be displayed. You should be able to see that the prometheus target is in the UP state.

### Adding custom-exporter port to prometheus

```bash
sudo /opt/prometheus/prometheus.yml
```

Create a new target by pasting this under '__static_configs:__'

```text
targets: [localhost:8085]
```

note(restart the prometheus service after adding the above lines to the yml file)

### Installing Arduino IDE

You can easily install Arduio IDE through the __Ubuntu software store__ or download the tar file through https://www.arduino.cc/en/software.

```bash
tar -xvf <package_name> #i.e arduino-1.6.10-linux64.tar.xz>
cd <directory_name> #i.e arduino-1.6.10>
./install.sh
ls -l /dev/ttyACM*
sudo usermod -a -G dialout <username>
```
![5](https://user-images.githubusercontent.com/84952780/173679075-4517eb62-2164-4ac2-b0be-f1904d3d025b.png)

## Usage

Check status of redis-server, prometheus and grafana by typing

```bash
sudo systemctl status <service_name>
```
At first you are required to manually set values in redis with the help of redis-cli, you can do that by

```bash
redis-cli
set <variable_name> <value>
```

If any service is not active you can start it by typing

```bash
sudo systemctl start <service_name>
```

(you can exit the code by pressing q)
(similarly start can be replaced by stop to disable the active service)

### Using esp32 with Arduino IDE



- Install arduino ide and add esp32 to the ide by adding the given url to __File__ > __Preferences__ > __setings__ > __Additional Boards Manager Urls__

```url
https://dl.espressif.com/dl/package_esp32_index.json, http://arduino.esp8266.com/stable/package_esp8266com_index.json
```

- Open the Boards Manager. Go to __Tools__ > __Board__ > __Boards Manager__...

- Search for esp32 and press install

- Now choose esp32 dev module in __Tools__ > __Board__ > __esp32 Arduino__

- Search and install __Adafruit MFRC630 RFID library__ and Wifi library from __Tools__ > __Manage Libraries__

- Open the esp32 file from your directory (or ctrl+O)

- Change __ssid__ and __password__ as per your Wifi.

- Changing the __server name__ in the esp32.ino file:
  - if on the same network:
```bash
ifconfig
```
  type in https then inet ip followed by ':8000', example: "https:/192.168.0.1:8000"

  - if different network you can do port forwarding with ngrok.
      type ngrok http -region in 8000 
    type in http forwarding link, example: "tcp://0.tcp.in.ngrok.io:18832"

    (note this ip will change everytime you run ngrok command so youll have to change the ino code.)

- __Compile__ and __upload__ the file on the esp32 module

### Staring fast-api

- Open your terminal and go to the cloned directory, inside the grafana_site directory and run

```bash
uvicorn main:app --host 0.0.0.0 --reload
```

this will start the rest api server

### Starting the prometheus node-exporter

```bash
python3 prom-exporter.py
```

- Open your web browser and head over to http://localhost:8085. You should be able to see some metrics being published over there by your prometheus exporter.

when you run the files at the first time you need to manually set value in redis, you can do that by

```bash
redis-cli
SET <your_metric_name> <value>
```

The metrics that you need to set here are: __tlimit__(for Time Limit), __esevices__(for Emergency Services, __vden__(for Vehicular Density)  (you can find these variables being defined in __main.py__ and used in __prom-exporter.py__)

You can also check the whether the values are being published or not by

```bash
redis-cli
GET <your_metric_name>
```

(you can exit it by pressing using exit command)

### Using Grafana

- Now you can log on to your grafana server, type in http://localhost:3000 in your browser. Both username and password would be 'admin'.

- Go to the __explore panel__ (Compas icon on the left)

- Now select __Metric Browser__>__Halt_total__ then select __your metric/job__.
file:///home/divi/Pictures/grafana.png![image](https://user-images.githubusercontent.com/84952780/174129103-674f4962-555e-42ad-991f-1a19d06c43e4.png)
file:///home/divi/Pictures/grafana1.png![image](https://user-images.githubusercontent.com/84952780/174129139-455fc0a9-feb7-487e-b070-9e7009cf126d.png)
 
 # FINAL PROJECT
![image](https://user-images.githubusercontent.com/84952780/173678644-d9f7fb6a-b36e-4b4c-9751-8affe7ae302e.png)


# OPEN CV
With the boom of artificial intelligence and machine learning, this project can be upgraded to the next level with the integration of computer vision (OpenCV). OpenCV works on image capturing technology and calculating the contour area for every frame and passing it on further. Based on the area of contours we can further manipulate the duration of green signal for subsequent cycles
![6](https://user-images.githubusercontent.com/84952780/170505446-30d74291-6b9c-4d08-a657-64af858bcfe1.png)
# STEPS
```bash
  pip install opencv
  cv2.imread()
  cv2.imshow() 
