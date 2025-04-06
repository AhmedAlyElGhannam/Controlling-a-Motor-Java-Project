all:
	javac -d build/ -cp ./jars/jSerialComm-2.11.0.jar:./jars/medusa-8.3.jar ./src/*.java
run:
	java -cp ./build:./jars/jSerialComm-2.11.0.jar:./jars/medusa-8.3.jar App
package:
	jar cfm MotorControl.jar MANIFEST.MF -C build . -C jars jSerialComm-2.11.0.jar -C jars medusa-8.3.jar
clean:
	rm ./build/*
