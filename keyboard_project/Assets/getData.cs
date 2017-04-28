using UnityEngine;
using System.Collections.Generic;
using UnityEngine.UI;
using System;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Threading;
using System.Globalization;

public class getData : MonoBehaviour {
    Thread socketThread;

    Queue<string> transMsgQueue = new Queue<string>();

    GameObject pc_keyboard;

    GameObject keyA, keyB, keyC, keyD, keyE, keyF, keyG, keyH, keyI, keyJ, keyK, keyL, keyM, keyN;
    GameObject keyO, keyP, keyQ, keyR, keyS, keyT, keyU, keyV, keyW, keyX, keyY, keyZ;

    GameObject keySpace, keyDel, keyEnter;

    public Text textInput;

    // Incoming data from the client.  
    public string data = null;

    public void StartListening() {
        // Data buffer for incoming data.  
        byte[] bytes = new Byte[1024];

        // Establish the local endpoint for the socket.  
        // Dns.GetHostName returns the name of the   
        // host running the application.  
        // IPHostEntry ipHostInfo = Dns.GetHostEntry(Dns.GetHostName());
        IPAddress ipAddress = IPAddress.Parse("0.0.0.0");
        IPEndPoint localEndPoint = new IPEndPoint(ipAddress, 5567);

        // Create a TCP/IP socket.  
        Socket listener = new Socket(AddressFamily.InterNetwork,
            SocketType.Stream, ProtocolType.Tcp);

        // Bind the socket to the local endpoint and   
        // listen for incoming connections.  
        try {
            listener.Bind(localEndPoint);
            listener.Listen(10);

            // Start listening for connections.  
            print("Waiting for a connection...");
            // Program is suspended while waiting for an incoming connection.  
            Socket handler = listener.Accept();
            print("Connection accepted!");

            data = null;
            // An incoming connection needs to be processed.  
            while (true) {
                bytes = new byte[1024];
                int bytesRec = handler.Receive(bytes);
                data += Encoding.ASCII.GetString(bytes, 0, bytesRec);
                if (data.Contains("#EOF#")) {
                    break;
                }
                // Show the data on the console.  
                print("Text received : " + data);

                transMsgQueue.Enqueue(data);
                print("Add message to transMsgQueue");

                // Echo the data back to the client.  
                byte[] msg = Encoding.ASCII.GetBytes(data);

                handler.Send(msg);
                data = null;
            }
            print("Connection End...");
            handler.Shutdown(SocketShutdown.Both);
            handler.Close();

        } catch (Exception e) {
            print(e.ToString());
        }

        print("\nPress ENTER to continue...");
        return;
    }

    // Use this for initialization
    void Start() {
        socketThread = new Thread(new ThreadStart(StartListening));
        socketThread.IsBackground = true;
        socketThread.Start();

        // whole keyboard
        pc_keyboard = GameObject.Find("/Classroom/Keyboard");
        // alpha keys
        keyA = GameObject.Find("/Classroom/Keyboard/pc_keyboard_a");
        keyB = GameObject.Find("/Classroom/Keyboard/pc_keyboard_b");
        keyC = GameObject.Find("/Classroom/Keyboard/pc_keyboard_c");
        keyD = GameObject.Find("/Classroom/Keyboard/pc_keyboard_d");
        keyE = GameObject.Find("/Classroom/Keyboard/pc_keyboard_e");
        keyF = GameObject.Find("/Classroom/Keyboard/pc_keyboard_f");
        keyG = GameObject.Find("/Classroom/Keyboard/pc_keyboard_g");
        keyH = GameObject.Find("/Classroom/Keyboard/pc_keyboard_h");
        keyI = GameObject.Find("/Classroom/Keyboard/pc_keyboard_i");
        keyJ = GameObject.Find("/Classroom/Keyboard/pc_keyboard_j");
        keyK = GameObject.Find("/Classroom/Keyboard/pc_keyboard_k");
        keyL = GameObject.Find("/Classroom/Keyboard/pc_keyboard_l");
        keyM = GameObject.Find("/Classroom/Keyboard/pc_keyboard_m");
        keyN = GameObject.Find("/Classroom/Keyboard/pc_keyboard_n");
        keyO = GameObject.Find("/Classroom/Keyboard/pc_keyboard_o");
        keyP = GameObject.Find("/Classroom/Keyboard/pc_keyboard_p");
        keyQ = GameObject.Find("/Classroom/Keyboard/pc_keyboard_q");
        keyR = GameObject.Find("/Classroom/Keyboard/pc_keyboard_r");
        keyS = GameObject.Find("/Classroom/Keyboard/pc_keyboard_s");
        keyT = GameObject.Find("/Classroom/Keyboard/pc_keyboard_t");
        keyU = GameObject.Find("/Classroom/Keyboard/pc_keyboard_u");
        keyV = GameObject.Find("/Classroom/Keyboard/pc_keyboard_v");
        keyW = GameObject.Find("/Classroom/Keyboard/pc_keyboard_w");
        keyX = GameObject.Find("/Classroom/Keyboard/pc_keyboard_x");
        keyY = GameObject.Find("/Classroom/Keyboard/pc_keyboard_y");
        keyZ = GameObject.Find("/Classroom/Keyboard/pc_keyboard_z");
        // special keys
        keySpace = GameObject.Find("/Classroom/Keyboard/pc_keyboard_space");
        keyDel = GameObject.Find("/Classroom/Keyboard/pc_keyboard_del");
        keyEnter = GameObject.Find("/Classroom/Keyboard/pc_keyboard_enter");
        // text
        textInput = GameObject.Find("/Classroom/group605/group309/computer130/screen400/Canvas/ScreenText").GetComponent<Text>();
    }

    // Update is called once per frame
    void Update() {
        string newChar = "";

        Vector3 pressAndRelease = new Vector3(0, 0.004f, 0);
        if (transMsgQueue.Count > 0) {
            print("Get message from transMsgQueue");
            string socketTransStr = transMsgQueue.Dequeue();
            string[] strArrays = socketTransStr.Split(',');
            float posX = float.Parse(strArrays[0], CultureInfo.InvariantCulture.NumberFormat);
            float posY = float.Parse(strArrays[1], CultureInfo.InvariantCulture.NumberFormat);
            float rotZ = float.Parse(strArrays[2], CultureInfo.InvariantCulture.NumberFormat);
            pc_keyboard.transform.position += new Vector3(posX/1500, 0, -posY/1500);
            pc_keyboard.transform.rotation *= Quaternion.Euler(0, 0, rotZ);
            // print(socketTransStr);
        }

        // key mapping for the key A
        if (Input.GetKeyDown(KeyCode.A)) {
            keyA.transform.position -= pressAndRelease;
            if (Input.GetKey(KeyCode.LeftShift) || Input.GetKey(KeyCode.RightShift)) {
                newChar = "A";
            } else {
                newChar = "a";
            }
        }
        if (Input.GetKeyUp(KeyCode.A)) {
            keyA.transform.position += pressAndRelease;
        }


        // key mapping for the key B
        if (Input.GetKeyDown(KeyCode.B)) {
            keyB.transform.position -= pressAndRelease;
            if (Input.GetKey(KeyCode.LeftShift) || Input.GetKey(KeyCode.RightShift)) {
                newChar = "B";
            } else {
                newChar = "b";
            }
        }
        if (Input.GetKeyUp(KeyCode.B)) {
            keyB.transform.position += pressAndRelease;
        }


        // key mapping for the key C
        if (Input.GetKeyDown(KeyCode.C)) {
            keyC.transform.position -= pressAndRelease;
            if (Input.GetKey(KeyCode.LeftShift) || Input.GetKey(KeyCode.RightShift)) {
                newChar = "C";
            } else {
                newChar = "c";
            }
        }
        if (Input.GetKeyUp(KeyCode.C)) {
            keyC.transform.position += pressAndRelease;
        }


        // key mapping for the key D
        if (Input.GetKeyDown(KeyCode.D)) {
            keyD.transform.position -= pressAndRelease;
            if (Input.GetKey(KeyCode.LeftShift) || Input.GetKey(KeyCode.RightShift)) {
                newChar = "D";
            } else {
                newChar = "d";
            }
        }
        if (Input.GetKeyUp(KeyCode.D)) {
            keyD.transform.position += pressAndRelease;
        }


        // key mapping for the key E
        if (Input.GetKeyDown(KeyCode.E)) {
            keyE.transform.position -= pressAndRelease;
            if (Input.GetKey(KeyCode.LeftShift) || Input.GetKey(KeyCode.RightShift)) {
                newChar = "E";
            } else {
                newChar = "e";
            }
        }
        if (Input.GetKeyUp(KeyCode.E)) {
            keyE.transform.position += pressAndRelease;
        }


        // key mapping for the key F
        if (Input.GetKeyDown(KeyCode.F)) {
            keyF.transform.position -= pressAndRelease;
            if (Input.GetKey(KeyCode.LeftShift) || Input.GetKey(KeyCode.RightShift)) {
                newChar = "F";
            } else {
                newChar = "f";
            }
        }
        if (Input.GetKeyUp(KeyCode.F)) {
            keyF.transform.position += pressAndRelease;
        }


        // key mapping for the key G
        if (Input.GetKeyDown(KeyCode.G)) {
            keyG.transform.position -= pressAndRelease;
            if (Input.GetKey(KeyCode.LeftShift) || Input.GetKey(KeyCode.RightShift)) {
                newChar = "G";
            } else {
                newChar = "g";
            }
        }
        if (Input.GetKeyUp(KeyCode.G)) {
            keyG.transform.position += pressAndRelease;
        }


        // key mapping for the key H
        if (Input.GetKeyDown(KeyCode.H)) {
            keyH.transform.position -= pressAndRelease;
            if (Input.GetKey(KeyCode.LeftShift) || Input.GetKey(KeyCode.RightShift)) {
                newChar = "H";
            } else {
                newChar = "h";
            }
        }
        if (Input.GetKeyUp(KeyCode.H)) {
            keyH.transform.position += pressAndRelease;
        }


        // key mapping for the key I
        if (Input.GetKeyDown(KeyCode.I)) {
            keyI.transform.position -= pressAndRelease;
            if (Input.GetKey(KeyCode.LeftShift) || Input.GetKey(KeyCode.RightShift)) {
                newChar = "I";
            } else {
                newChar = "i";
            }
        }
        if (Input.GetKeyUp(KeyCode.I)) {
            keyI.transform.position += pressAndRelease;
        }


        // key mapping for the key J
        if (Input.GetKeyDown(KeyCode.J)) {
            keyJ.transform.position -= pressAndRelease;
            if (Input.GetKey(KeyCode.LeftShift) || Input.GetKey(KeyCode.RightShift)) {
                newChar = "J";
            } else {
                newChar = "j";
            }
        }
        if (Input.GetKeyUp(KeyCode.J)) {
            keyJ.transform.position += pressAndRelease;
        }


        // key mapping for the key K
        if (Input.GetKeyDown(KeyCode.K)) {
            keyK.transform.position -= pressAndRelease;
            if (Input.GetKey(KeyCode.LeftShift) || Input.GetKey(KeyCode.RightShift)) {
                newChar = "K";
            } else {
                newChar = "k";
            }
        }
        if (Input.GetKeyUp(KeyCode.K)) {
            keyK.transform.position += pressAndRelease;
        }


        // key mapping for the key L
        if (Input.GetKeyDown(KeyCode.L)) {
            keyL.transform.position -= pressAndRelease;
            if (Input.GetKey(KeyCode.LeftShift) || Input.GetKey(KeyCode.RightShift)) {
                newChar = "L";
            } else {
                newChar = "l";
            }
        }
        if (Input.GetKeyUp(KeyCode.L)) {
            keyL.transform.position += pressAndRelease;
        }


        // key mapping for the key M
        if (Input.GetKeyDown(KeyCode.M)) {
            keyM.transform.position -= pressAndRelease;
            if (Input.GetKey(KeyCode.LeftShift) || Input.GetKey(KeyCode.RightShift)) {
                newChar = "M";
            } else {
                newChar = "m";
            }
        }
        if (Input.GetKeyUp(KeyCode.M)) {
            keyM.transform.position += pressAndRelease;
        }


        // key mapping for the key N
        if (Input.GetKeyDown(KeyCode.N)) {
            keyN.transform.position -= pressAndRelease;
            if (Input.GetKey(KeyCode.LeftShift) || Input.GetKey(KeyCode.RightShift)) {
                newChar = "N";
            } else {
                newChar = "n";
            }
        }
        if (Input.GetKeyUp(KeyCode.N)) {
            keyN.transform.position += pressAndRelease;
        }


        // key mapping for the key O
        if (Input.GetKeyDown(KeyCode.O)) {
            keyO.transform.position -= pressAndRelease;
            if (Input.GetKey(KeyCode.LeftShift) || Input.GetKey(KeyCode.RightShift)) {
                newChar = "O";
            } else {
                newChar = "o";
            }
        }
        if (Input.GetKeyUp(KeyCode.O)) {
            keyO.transform.position += pressAndRelease;
        }


        // key mapping for the key P
        if (Input.GetKeyDown(KeyCode.P)) {
            keyP.transform.position -= pressAndRelease;
            if (Input.GetKey(KeyCode.LeftShift) || Input.GetKey(KeyCode.RightShift)) {
                newChar = "P";
            } else {
                newChar = "p";
            }
        }
        if (Input.GetKeyUp(KeyCode.P)) {
            keyP.transform.position += pressAndRelease;
        }


        // key mapping for the key Q
        if (Input.GetKeyDown(KeyCode.Q)) {
            keyQ.transform.position -= pressAndRelease;
            if (Input.GetKey(KeyCode.LeftShift) || Input.GetKey(KeyCode.RightShift)) {
                newChar = "Q";
            } else {
                newChar = "q";
            }
        }
        if (Input.GetKeyUp(KeyCode.Q)) {
            keyQ.transform.position += pressAndRelease;
        }


        // key mapping for the key R
        if (Input.GetKeyDown(KeyCode.R)) {
            keyR.transform.position -= pressAndRelease;
            if (Input.GetKey(KeyCode.LeftShift) || Input.GetKey(KeyCode.RightShift)) {
                newChar = "R";
            } else {
                newChar = "r";
            }
        }
        if (Input.GetKeyUp(KeyCode.R)) {
            keyR.transform.position += pressAndRelease;
        }


        // key mapping for the key S
        if (Input.GetKeyDown(KeyCode.S)) {
            keyS.transform.position -= pressAndRelease;
            if (Input.GetKey(KeyCode.LeftShift) || Input.GetKey(KeyCode.RightShift)) {
                newChar = "S";
            } else {
                newChar = "s";
            }
        }
        if (Input.GetKeyUp(KeyCode.S)) {
            keyS.transform.position += pressAndRelease;
        }


        // key mapping for the key T
        if (Input.GetKeyDown(KeyCode.T)) {
            keyT.transform.position -= pressAndRelease;
            if (Input.GetKey(KeyCode.LeftShift) || Input.GetKey(KeyCode.RightShift)) {
                newChar = "T";
            } else {
                newChar = "t";
            }
        }
        if (Input.GetKeyUp(KeyCode.T)) {
            keyT.transform.position += pressAndRelease;
        }


        // key mapping for the key U
        if (Input.GetKeyDown(KeyCode.U)) {
            keyU.transform.position -= pressAndRelease;
            if (Input.GetKey(KeyCode.LeftShift) || Input.GetKey(KeyCode.RightShift)) {
                newChar = "U";
            } else {
                newChar = "u";
            }
        }
        if (Input.GetKeyUp(KeyCode.U)) {
            keyU.transform.position += pressAndRelease;
        }


        // key mapping for the key V
        if (Input.GetKeyDown(KeyCode.V)) {
            keyV.transform.position -= pressAndRelease;
            if (Input.GetKey(KeyCode.LeftShift) || Input.GetKey(KeyCode.RightShift)) {
                newChar = "V";
            } else {
                newChar = "v";
            }
        }
        if (Input.GetKeyUp(KeyCode.V)) {
            keyV.transform.position += pressAndRelease;
        }


        // key mapping for the key W
        if (Input.GetKeyDown(KeyCode.W)) {
            keyW.transform.position -= pressAndRelease;
            if (Input.GetKey(KeyCode.LeftShift) || Input.GetKey(KeyCode.RightShift)) {
                newChar = "W";
            } else {
                newChar = "w";
            }
        }
        if (Input.GetKeyUp(KeyCode.W)) {
            keyW.transform.position += pressAndRelease;
        }


        // key mapping for the key X
        if (Input.GetKeyDown(KeyCode.X)) {
            keyX.transform.position -= pressAndRelease;
            if (Input.GetKey(KeyCode.LeftShift) || Input.GetKey(KeyCode.RightShift)) {
                newChar = "X";
            } else {
                newChar = "x";
            }
        }
        if (Input.GetKeyUp(KeyCode.X)) {
            keyX.transform.position += pressAndRelease;
        }


        // key mapping for the key Y
        if (Input.GetKeyDown(KeyCode.Y)) {
            keyY.transform.position -= pressAndRelease;
            if (Input.GetKey(KeyCode.LeftShift) || Input.GetKey(KeyCode.RightShift)) {
                newChar = "Y";
            } else {
                newChar = "y";
            }
        }
        if (Input.GetKeyUp(KeyCode.Y)) {
            keyY.transform.position += pressAndRelease;
        }


        // key mapping for the key Z
        if (Input.GetKeyDown(KeyCode.Z)) {
            keyZ.transform.position -= pressAndRelease;
            if (Input.GetKey(KeyCode.LeftShift) || Input.GetKey(KeyCode.RightShift)) {
                newChar = "Z";
            } else {
                newChar = "z";
            }
        }
        if (Input.GetKeyUp(KeyCode.Z)) {
            keyZ.transform.position += pressAndRelease;
        }

        // Special keys
        if (Input.GetKeyDown(KeyCode.Space)) {
            keySpace.transform.position -= pressAndRelease;
            newChar = " ";
        }
        if (Input.GetKeyUp(KeyCode.Space)) {
            keySpace.transform.position += pressAndRelease;
        }

        // enter
        if (Input.GetKeyDown(KeyCode.Return)) {
            keyEnter.transform.position -= pressAndRelease;
            newChar = "\n";
        }
        if (Input.GetKeyUp(KeyCode.Return)) {
            keyEnter.transform.position += pressAndRelease;
        }

        // update text
        textInput.text += newChar;

        // del
        if (Input.GetKeyDown(KeyCode.Backspace)) {
            keyDel.transform.position -= pressAndRelease;
            if (textInput.text.Length > 0) {
                textInput.text = textInput.text.Substring(0, textInput.text.Length - 1);
            }
        }
        if (Input.GetKeyUp(KeyCode.Backspace)) {
            keyDel.transform.position += pressAndRelease;
        }

    }
}
