

////////////////////////////////////////////








	
	
	//macAddress from the zuzuhack bluetooth chipset
	//TODO: set it from a settings menu
	var macAddress = "98:D3:31:10:06:60";
	//var macAddress_JJ = "00:14:01:06:14:63";
	//var macAddress_cb = "98:D3:31:10:06:60";
	//var macAddress = "00:06:66:4D:AA:AA";
	
	

	
	
	//Global data used to toogle led, debug purpose
	var ledStatus="1";

	


	
	function play(id){
	    //force connection if needed
		connect();
		
		console.log("Song..."+id);
		sendToArduino("p",id);
		Toast.shortshow("Play Song"+id);
	}
	
	function motor(id){
	    //force connection if needed
		connect();
		
		console.log("Move out!"+id);
		sendToArduino("m",id);
		Toast.shortshow("Bouge de là! "+id);
	}	
	
	function nooze(id){
	    //force connection if needed
		connect();
		
		console.log("Change Noose status:"+id);
		sendToArduino("n",id);
		Toast.shortshow("Nez"+id);
	}	
	
	
	
	function connect(e){
		console.log("connect to zuzpet...");
		//Toast.shortshow("Plugin called successfully :-)");
	}
	
	//Used for debug purpose only: toggle arduino led
	function toggleLED(e){
		if (ledStatus=="1") {
			ledStatus="0";
		} else {
			ledStatus="1";
		};
		console.log("ledStatus:"+ledStatus);  
		sendToArduino("d",ledStatus);	
	}	
	

	//Used for debug purpose only: toggle arduino led
	//Tcs
	//d : debug : used to set led status , val=0/1
	//p : play song #val , val=1..3
    function sendToArduino(cmd,val) {
        BluetoothSerial.write(cmd + val + "\n");
    }
	
	
	function connect() {  
        console.log("connect");      
        BluetoothSerial.isConnected(function (connected) {
            console.log("connected is " + connected);
            if (!connected) {
                console.log("connecting");                                      
                BluetoothSerial.connect(
                    macAddress, 
                    function() { console.log("connected"); }, 
                    function (error) { console.log("error"); }
                );        
            } else {
                console.log("already connected");
            }            
        });
    }
	
	
	function onDeviceReady() {
		$("#sayHello").click(sayHello);
		desiredWidth = window.innerWidth;
		desiredHeight = (window.innerHeight)/2;
		alert('Hi Guy! ');	
		
		
	};
	
	function init() {
		document.addEventListener("deviceready", onDeviceReady, true);
	} 










   // `rgbToHex`
    // Converts an RGB color to hex
    // Assumes r, g, and b are contained in the set [0, 255]
    // Returns a 6 character hex
    function rgbToHex(r, g, b) {

        var hex = [
            r.toString(16),
            g.toString(16),
            b.toString(16)
        ];


        return hex.join("");
    }
