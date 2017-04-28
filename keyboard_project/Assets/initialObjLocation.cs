using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using Leap;

public class initialObjLocation : MonoBehaviour {

    GameObject pc_keyboard;
    GameObject pc_desktop;

    private int presetCnt = 0;

    private SteamVR_TrackedObject trackedObj;
    private SteamVR_Controller.Device Controller {
        get { return SteamVR_Controller.Input((int)trackedObj.index); }
    }

    void Awake() {
        trackedObj = GetComponent<SteamVR_TrackedObject>();
        pc_keyboard = GameObject.Find("/Classroom/Keyboard");
        pc_desktop = GameObject.Find("/Classroom/group605");
    }

	// Update is called once per frame
	void Update () {
        if (++presetCnt <= 10) {
            Vector3 ctrlPos = Controller.transform.pos;
            Vector3 ctrlRot = Controller.transform.rot.eulerAngles;
            // rotate 180 around y axis
            // 0.02, 0.04, 0.035
            Vector3 realCtrlPos = new Vector3(-ctrlPos.x, ctrlPos.y - 0.039f, -ctrlPos.z + 0.05f) + new Vector3(-4.67f, 0, 1.68f);
            pc_keyboard.transform.position = realCtrlPos;
            // rotation
            Vector3 keyboardRot = new Vector3(-90, ctrlRot.y, 0);
            pc_keyboard.transform.rotation = Quaternion.Euler(keyboardRot);
            // controller height vector
            Vector3 realDestop = new Vector3(pc_desktop.transform.position.x, ctrlPos.y + 0.4f, pc_desktop.transform.position.z);
            pc_desktop.transform.position = realDestop;
        }
        // print(pc_keyboard.transform.position
    }
}
