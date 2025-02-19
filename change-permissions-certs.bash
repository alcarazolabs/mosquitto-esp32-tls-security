#!/bin/bash
# 1. Dar permisos de ejecucion a este script: 
# sudo chmod +x change-permissions-certs.bash
# 2. Ejecutar con sudo:
# sudo ./change-permissions-certs.bash


echo "changing to mosquitto:mosquitto group.."

chown mosquitto:mosquitto /etc/mosquitto/certs/ca.crt
chown mosquitto:mosquitto /etc/mosquitto/certs/server.crt
chown mosquitto:mosquitto /etc/mosquitto/certs/server.key

echo "1. Group changed (ok).."

echo "adding lecture permissions"

chmod 664 /etc/mosquitto/certs/ca.crt
chmod 664 /etc/mosquitto/certs/server.crt
chmod 664 /etc/mosquitto/certs/server.key

echo "2. lecture permissions done (ok)."
echo "run ls -l /etc/mosquitto/certs/ to check!.."
echo "end!" 
