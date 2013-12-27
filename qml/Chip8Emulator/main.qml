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
		onClicked:chip8.running ? chip8.play(0):chip8.play(1);
	    }
	}



	Row{
	    spacing: 50
	    Column{
		Row{Text {text: " op "} TextInput{ text: chip8.op.toString(16); onAccepted:{ chip8.op = parseInt("0x"+text) } } }
		Row{Text {text: " pc "} TextInput{ text: chip8.pc.toString(16); onAccepted:{ chip8.pc = parseInt("0x"+text) } } }
		Row{Text {text: " I  "} TextInput{ text: chip8.i.toString(16);  onAccepted:{ chip8.i  = parseInt("0x"+text) } } }
		Row{Text {text: " v0 "} TextInput{ text: chip8.v0.toString(16); onAccepted:{ chip8.v0 = parseInt("0x"+text) } } }
		Row{Text {text: " v1 "} TextInput{ text: chip8.v1.toString(16); onAccepted:{ chip8.v1 = parseInt("0x"+text) } } }
		Row{Text {text: " v2 "} TextInput{ text: chip8.v2.toString(16); onAccepted:{ chip8.v2 = parseInt("0x"+text) } } }
		Row{Text {text: " v3 "} TextInput{ text: chip8.v3.toString(16); onAccepted:{ chip8.v3 = parseInt("0x"+text) } } }
		Row{Text {text: " v4 "} TextInput{ text: chip8.v4.toString(16); onAccepted:{ chip8.v4 = parseInt("0x"+text) } } }
		Row{Text {text: " v5 "} TextInput{ text: chip8.v5.toString(16); onAccepted:{ chip8.v5 = parseInt("0x"+text) } } }
		Row{Text {text: " v6 "} TextInput{ text: chip8.v6.toString(16); onAccepted:{ chip8.v6 = parseInt("0x"+text) } } }
		Row{Text {text: " v7 "} TextInput{ text: chip8.v7.toString(16); onAccepted:{ chip8.v7 = parseInt("0x"+text) } } }
		Row{Text {text: " v8 "} TextInput{ text: chip8.v8.toString(16); onAccepted:{ chip8.v8 = parseInt("0x"+text) } } }
		Row{Text {text: " v9 "} TextInput{ text: chip8.v9.toString(16); onAccepted:{ chip8.v9 = parseInt("0x"+text) } } }
		Row{Text {text: " vA "} TextInput{ text: chip8.vA.toString(16); onAccepted:{ chip8.vA = parseInt("0x"+text) } } }
		Row{Text {text: " vB "} TextInput{ text: chip8.vB.toString(16); onAccepted:{ chip8.vB = parseInt("0x"+text) } } }
		Row{Text {text: " vC "} TextInput{ text: chip8.vC.toString(16); onAccepted:{ chip8.vC = parseInt("0x"+text) } } }
		Row{Text {text: " vD "} TextInput{ text: chip8.vD.toString(16); onAccepted:{ chip8.vD = parseInt("0x"+text) } } }
		Row{Text {text: " vE "} TextInput{ text: chip8.vE.toString(16); onAccepted:{ chip8.vE = parseInt("0x"+text) } } }
		Row{Text {text: " vF "} TextInput{ text: chip8.vF.toString(16); onAccepted:{ chip8.vF = parseInt("0x"+text) } } }
	    }

	    ListView{
		id:memoryView
		width: 200
		height: 200
		model:4096/2
		clip:true
		delegate:  Row{
		    Text {text: " "+(index*2).toString(16)+" : "}
		    TextInput{id:value; text: chip8.readWordMemoryAt(index*2).toString(16); onAccepted:{ chip8.writeWordInMemoryAt( parseInt("0x"+text), index*2) } }
		    Connections{ target: chip8; onStepping: value.text=chip8.readWordMemoryAt((index*2  )).toString(16); }
		}
	    }
	}
    }
}


