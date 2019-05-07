using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class Audio : MonoBehaviour
{
    public static AudioClip MusicClip;
    public static AudioClip MusicClip1;
    public static AudioClip MusicClip2;
    public static AudioClip MusicClip3;
    public static AudioClip MusicClip4;
    public static AudioClip MusicClip5;

     static AudioSource MusicSource;
  
    // Start is called before the first frame update
   void Start()
    {
        
        MusicClip = Resources.Load<AudioClip>("one1557173579");
        MusicClip1 = Resources.Load<AudioClip>("two1557173623");
        MusicClip2 = Resources.Load<AudioClip>("three1557173638");
        MusicClip3 = Resources.Load<AudioClip>("four1557173660");
        MusicClip4 = Resources.Load<AudioClip>("five1557173679");
        MusicClip5 = Resources.Load<AudioClip>("six1557173696");

        MusicSource = GetComponent<AudioSource>();
    }

        void update()
    {

    }
    // Update is called once per frame
    public static void pal(int y)
    {
        if (y == 0)
        {
            MusicSource.PlayOneShot(MusicClip);

        }
        else if (y == 1)
        {
            MusicSource.PlayOneShot(MusicClip1);

        }
        else if (y == 2)
        {
            MusicSource.PlayOneShot(MusicClip2);
        }
        else if (y == 3)
        {
            MusicSource.PlayOneShot(MusicClip3);

        }
        else if (y == 4)
        {
            MusicSource.PlayOneShot(MusicClip4);

        }
        else
        {
            MusicSource.PlayOneShot(MusicClip5);

        }
        
            
        
      

    }
}
