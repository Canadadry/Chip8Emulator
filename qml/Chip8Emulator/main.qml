import QtQuick 2.0
import Chip8 1.0

Emulator {
    id:root
    width: 64*pixelSize
    height: 32*pixelSize
    property int  pixelSize: 20;
    Timer{
	id:timer
	interval: 1000
	onTriggered: step();
	repeat: true
    }
    MouseArea{
	anchors.fill: parent
	onClicked: {
	    if(timer.running == false )
	    {
		root.loadROM();
		timer.start();
	    }
	    else
	    {
	       timer.stop();
	    }
	}
    }

}
