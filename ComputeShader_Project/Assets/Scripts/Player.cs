using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Player : MonoBehaviour
{
    public float turnSpeed =120f;
    public float Speed = 12f;

    // Start is called before the first frame update
    void Start()
    {
        
    }

    // Update is called once per frame
    void Update()
    {
        transform.Rotate(0, turnSpeed * Input.GetAxis("Horizontal") * Time.deltaTime, 0f, Space.World);
        transform.Translate(Speed * Input.GetAxis("Vertical") * Time.deltaTime, 0f, 0f, Space.Self);
    }
}
