#!/bin/bash

# 1. Cambiar la IP por su IP o agregar dominio.
# 2. Agregar el numero de dias de vencimiento ver -days 1 por ejemplo 365.
# 3. Dar permisos de ejecucion al script:
# sudo chmod +x tls-certs-script.bash
# 4. Ejecutar:
# sudo ./ls-certs-script.bash

IP="192.168.18.149"
SUBJECT_CA="/C=SE/ST=Stockholm/L=Stockholm/O=himinds/OU=CA/CN=$IP"
SUBJECT_SERVER="/C=SE/ST=Stockholm/L=Stockholm/O=himinds/OU=Server/CN=$IP"
SUBJECT_CLIENT="/C=SE/ST=Stockholm/L=Stockholm/O=himinds/OU=Client/CN=$IP"

function generate_CA () {
   echo "$SUBJECT_CA"
   openssl req -x509 -nodes -sha256 -newkey rsa:2048 -subj "$SUBJECT_CA"  -days 1 -keyout ca.key -out ca.crt
}

function generate_server () {
   echo "$SUBJECT_SERVER"
   openssl req -nodes -sha256 -new -subj "$SUBJECT_SERVER" -keyout server.key -out server.csr
   openssl x509 -req -sha256 -in server.csr -CA ca.crt -CAkey ca.key -CAcreateserial -out server.crt -days 1
}

function generate_client () {
   echo "$SUBJECT_CLIENT"
   openssl req -new -nodes -sha256 -subj "$SUBJECT_CLIENT" -out client.csr -keyout client.key 
   openssl x509 -req -sha256 -in client.csr -CA ca.crt -CAkey ca.key -CAcreateserial -out client.crt -days 1
}

function copy_keys_to_broker () {
   sudo cp ca.crt /etc/mosquitto/certs/
   sudo cp server.crt /etc/mosquitto/certs/
   sudo cp server.key /etc/mosquitto/certs/
}

generate_CA
generate_server
generate_client
copy_keys_to_broker
