import QtQuick 2.0
import Chip8 1.0


Column {
    id: root

    Emulator {
	id:chip8
	width: 64*pixelSize
	height: 32*pixelSize
	property int  pixelSize: 5;
	signal stepping

	MouseArea{
	    anchors.fill: parent
	    property bool loaded: false
	    onClicked: {
		if(!loaded)
		{
		    chip8.loadROM();
		    memoryView.positionViewAtIndex(0x100,ListView.Beginning)
		    loaded = true;
		    chip8.stepping();
		}
		else
		{
		    chip8.step();
		    chip8.stepping();
		}
	    }
	}
    }
    Column{
	Rectangle{
	    color: "red"
	    width: root.width
	    height: 30
	    Text {
		anchors.centerIn: parent
		text:chip8.running ? "playing":"pause"
	    }
	    MouseArea{
		anchors.fill: parent
		onClicked:chip8.running ? chip8.play(0):chip8.play(10);
	    }
	}

	Row{
	    id:register
	    spacing: 50
	    Column{
		Repeater{
		    model:["op","pc","i","v0","v1","v2","v3","v4","v5","v6","v7","v8","v9","vA","vB","vC","vD","vE","vF"]
		    delegate:Row{
			Text {text: " "+modelData+" ";width: 25}
			Text {text: ": "}
			TextInput{
			    text: eval("chip8."+modelData+".toString(16)")
			    onAccepted: eval("chip8."+modelData+" = parseInt(\"0x\"+text)")  }
		    }
		}
	    }

	    ListView{
		id:memoryView
		width: root.width/2
		height: register.height
		model:4096/2
		clip:true
		delegate:  Row{
		    Text {text: " "+(index*2).toString(16);width: 25}
		    Text {text: ": "}
		    TextInput{id:value; text: chip8.readWordMemoryAt(index*2).toString(16); onAccepted:{ chip8.writeWordInMemoryAt( parseInt("0x"+text), index*2) } }
		    Connections{ target: chip8; onStepping: value.text=chip8.readWordMemoryAt((index*2  )).toString(16); }
		}
	    }
	}
    }
}


