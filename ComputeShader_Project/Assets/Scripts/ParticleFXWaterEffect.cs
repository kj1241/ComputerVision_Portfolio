using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class VFXWaterEffect : MonoBehaviour
{
    public ParticleSystem part;
    public List<ParticleCollisionEvent> collisionEvents;

    const int maxnumber = 8;//มฆวั

    // Start is called before the first frame update
    void Start()
    {
        part = GetComponent<ParticleSystem>();
        collisionEvents = new List<ParticleCollisionEvent>();
    }

    // Update is called once per frame
    void Update()
    {

    }

    void OnParticleCollision(GameObject other)
    {
        int numCollisionEvents = part.GetCollisionEvents(other, collisionEvents);
        WaterManger waterManger = other.GetComponent<WaterManger>();

        Debug.Log(waterManger);
        Debug.Log(other.tag);

        if (other.transform.tag == "water")
        {
            int i = 0;
            while (i < numCollisionEvents)
            {
                if (i > maxnumber || !waterManger)
                    break;
                waterManger.effect = new Vector3( collisionEvents[i].intersection.x , collisionEvents[i].intersection.z, 1);
                ++i;
            }
        }
    }
}
