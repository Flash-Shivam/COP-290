using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class f : MonoBehaviour
{
    // Start is called before the first frame update

    public void Dis(int x)
    {
        if (x == 1)
        {
            a.rec = "CORRECT";
        }
        else
        {
            a.rec = "WRONG";
        }

    }
}
