using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class DiceCheckNumber : MonoBehaviour
{
    // Start is called before the first frame update
    Text text;
    public static int diceNumber;

    // Use this for initialization
    void Start()
    {
        text = GetComponent<Text>();
    }
    

    // Update is called once per frame
    void Update()
    {
        text.text = diceNumber.ToString();
    }
}
