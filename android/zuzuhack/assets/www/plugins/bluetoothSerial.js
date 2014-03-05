/*global cordova*/
cordova.define('cordova/plugin/bluetoothSerial', function (require, exports, module) {

	var exec = require("cordova/exec");
	var BluetoothSerial = function() {};
	
	BluetoothSerial.prototype.connect = function(macAddress,successCallback,failureCallback) {
	    exec(successCallback, failureCallback, 'BluetoothSerial', 'connect', [macAddress]);
	}
	
	// writes data to the bluetooth serial port - data must be a string
	BluetoothSerial.prototype.write = function(data,successCallback,failureCallback) {
	    exec(successCallback, failureCallback, 'BluetoothSerial', 'write', [data]);
	}
   
 	// writes data to the bluetooth serial port - data must be a string
	BluetoothSerial.prototype.isConnected = function(successCallback,failureCallback) {
	    exec(successCallback, failureCallback, 'BluetoothSerial', 'isConnected', []);
	}  
   
   

	
    var bluetoothSerial = new BluetoothSerial();
    module.exports = bluetoothSerial;	
});

if (!window.plugins) {
    window.plugins = {};
}

if (!window.plugins.BluetoothSerial) {
    window.plugins.BluetoothSerial = cordova.require("cordova/plugin/bluetoothSerial");
    window.BluetoothSerial = window.plugins.BluetoothSerial;
}



