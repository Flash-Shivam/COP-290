using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class a : MonoBehaviour
{
    Text text;
    public static string rec = "";
    // Start is called before the first frame update
    void Start()
    {
        text = GetComponent<Text>();
    }


    // Update is called once per frame
    void Update()
    {
        text.text = rec;
    }
}
