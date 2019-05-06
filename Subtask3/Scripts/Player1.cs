using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Player1 : MonoBehaviour
{
    // Start is called before the first frame update
    /* public Rigidbody rb;
     public float speed;
     // Update is called once per frame

     void Start()
     {
         rb = GetComponent<Rigidbody>();
     }
     void FixedUpdate()
     {
         float mHorizontal = Input.GetAxis("Horizontal");
         float mVertical = Input.GetAxis("Vertical");

         Vector3 movement = new Vector3(mHorizontal, 0.00f, mVertical);

         rb.AddForce(movement);
     }*/

    [SerializeField]
    Transform[] waypoints;

    [SerializeField]
    float moveSpeed = 2f;

    int waypointIndex = 0;

    void Start()
    {
        transform.position = waypoints[waypointIndex].transform.position;
    }

    void Update()
    {
        Move();
    }

    void Move()
    {
        transform.position = Vector2.MoveTowards(transform.position,
                                                waypoints[waypointIndex].transform.position,
                                                moveSpeed * Time.deltaTime);

        if (transform.position == waypoints[waypointIndex].transform.position)
        {
            waypointIndex += 1;
        }

        if (waypointIndex == waypoints.Length)
            waypointIndex = 0;
    }
}
