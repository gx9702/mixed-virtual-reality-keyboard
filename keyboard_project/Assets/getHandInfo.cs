using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using Leap;

public class getHandInfo : MonoBehaviour {

    private Controller leapController;
    private GameObject handRPalm;
    private GameObject handRPinchPoint;

    private GameObject handLPalm;
    private GameObject handLPinchPoint;


    GameObject pc_keyboard;
    GameObject pc_desktop;

    // Use this for initialization
    void Start () {
        leapController = new Controller();
        pc_keyboard = GameObject.Find("/Classroom/Keyboard");
        pc_desktop = GameObject.Find("/Classroom/group605");
    }
	
	// Update is called once  per frame
	void Update () {
        Vector3 handRPalmPos = new Vector3();
        Vector3 handLPalmPos = new Vector3();
        Vector3 handRPinchPointPos = new Vector3();
        Vector3 handLPinchPointPos = new Vector3();
        bool doPalmL = false;
        bool doPalmR = false;
        bool doPinchL = false;
        bool doPinchR = false;
        if (leapController.IsConnected) {
            Frame leapFrame = leapController.Frame();
            List<Hand> leapHands = leapFrame.Hands;
            for (int idx = 0; idx < leapHands.Count; idx++) {
                Hand tmpHand = leapHands[idx];
                if (tmpHand.IsRight) {
                    handRPinchPoint = GameObject.FindGameObjectWithTag("HandRPinchPoint");
                    handRPinchPointPos = handRPinchPoint.transform.position;

                    handRPalm = GameObject.FindGameObjectWithTag("HandRPalm");
                    handRPalmPos = handRPalm.transform.position;
                    if (tmpHand.PinchStrength == 0) {
                        doPalmR = true;
                    }
                    if (tmpHand.PinchStrength >= 0.9) {
                        doPinchR = true;
                    }

                }
                if (tmpHand.IsLeft) {
                    handLPinchPoint = GameObject.FindGameObjectWithTag("HandLPinchPoint");
                    handLPinchPointPos = handLPinchPoint.transform.position;

                    //handLPalm = GameObject.FindGameObjectWithTag("HandLPalm");
                    //handLPalmPos = handLPalm.transform.position;
                    if (tmpHand.PinchStrength == 0) {
                        doPalmL = true;
                    }
                    if (tmpHand.PinchStrength == 1) {
                        doPinchL = true;
                    }
                }

                if (doPinchL && doPalmR && (handLPinchPointPos.y - handRPalmPos.y > 0.1)) {
                    pc_keyboard.transform.position = new Vector3(handLPinchPointPos.x, handRPalmPos.y - 0.01f, handLPinchPointPos.z);
                    pc_desktop.transform.position = new Vector3(pc_desktop.transform.position.x, handRPalmPos.y + 0.42f, pc_desktop.transform.position.z);
                }
            }
        }
    }
}
