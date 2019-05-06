using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class Button1 : MonoBehaviour
{
    // Start is called before the first frame update
    public void Display(int x)
    {
        if(x == 0)
        {
            a.rec = "CORRECT";
        }
        else
        {
            a.rec = "WRONG";
        }
        
    }
}
