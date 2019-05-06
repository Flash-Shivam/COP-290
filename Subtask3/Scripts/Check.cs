using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class Check : MonoBehaviour
{
    // Start is called before the first frame update
    Vector3 diceVelocity;

    // Update is called once per frame
    void FixedUpdate()
    {
        diceVelocity = Dice.diceVelocity;
    }

    void OnTriggerStay(Collider col)
    {
        if (diceVelocity.x == 0f && diceVelocity.y == 0f && diceVelocity.z == 0f)
        {
            switch (col.gameObject.name)
            {
                case "Side1":
                    DiceCheckNumber.diceNumber = 6;
                    break;
                case "Side2":
                    DiceCheckNumber.diceNumber = 5;
                    break;
                case "Side3":
                    DiceCheckNumber.diceNumber = 4;
                    break;
                case "Side4":
                    DiceCheckNumber.diceNumber = 3;
                    break;
                case "Side5":
                    DiceCheckNumber.diceNumber = 2;
                    break;
                case "Side6":
                    DiceCheckNumber.diceNumber = 1;
                    break;
            }
        }
    }

}
