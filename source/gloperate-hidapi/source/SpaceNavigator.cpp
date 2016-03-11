
#include <gloperate-hidapi/SpaceNavigator.h>

#include <hidapi/hidapi.h>
#include <iostream>


namespace gloperate_hidapi
{
SpaceNavigator::SpaceNavigator(gloperate::InputManager * inputManager, const std::string & deviceDescriptor, const std::string& path)
: AbstractDevice(inputManager, deviceDescriptor)
{
    m_pHandle = nullptr;
    m_pHandle = hid_open_path(path.c_str());
}

SpaceNavigator::~SpaceNavigator()
{
    if(m_pHandle != nullptr)
    {
        hid_close(m_pHandle);
    }
}

void SpaceNavigator::update()
{
    unsigned char buf[65];
    int readBytes = 0;
    if(m_pHandle)
    {
        readBytes = hid_read_timeout(m_pHandle, buf, 64, m_timeout);
        if(readBytes == -1)
            return;
    }

    float transX = 0;
    float transY = 0;
    float transZ = 0;
    float rotX = 0;
    float rotY = 0;
    float rotZ = 0;

    switch(buf[0])
    {
        case 0x01:
        {
            transX = static_cast<float>(static_cast<int16_t>((buf[1] & 0x000000ff) | (static_cast<int16_t>(buf[2])<<8 & 0xffffff00)));
            transY = static_cast<float>(static_cast<int16_t>((buf[3] & 0x000000ff) | (static_cast<int16_t>(buf[4])<<8 & 0xffffff00)));
            transZ = static_cast<float>(static_cast<int16_t>((buf[5] & 0x000000ff) | (static_cast<int16_t>(buf[6])<<8 & 0xffffff00)));


            std::cout<<"Rotation" << transX << " " << transY << " " << transZ << std::endl;
            break;


            break;
        }

        case 0x02:
        {
            rotX = static_cast<float>(static_cast<int16_t>((buf[1] & 0x000000ff) | (static_cast<int16_t>(buf[2])<<8 & 0xffffff00)));
            rotY = static_cast<float>(static_cast<int16_t>((buf[3] & 0x000000ff) | (static_cast<int16_t>(buf[4])<<8 & 0xffffff00)));
            rotZ = static_cast<float>(static_cast<int16_t>((buf[5] & 0x000000ff) | (static_cast<int16_t>(buf[6])<<8 & 0xffffff00)));

            std::cout<<"Rotation" << rotX << " " << rotY << " " << rotZ << std::endl;
            break;
        }

        // Buttons
        case 0x03:
        {
            //bool bPressed = ((buf[1] & 0x0001) != 0);
            //m_control.setPressed(bPressed);
            /*
            bPressed = ((pInputBuffer[1] & 0x0002) != 0);
            if (Button1.IsPressed() != bPressed)
                Button1.SetPressed(bPressed);
            bPressed = ((pInputBuffer[1] & 0x0004) != 0);
            if (Button2.IsPressed() != bPressed)
                Button2.SetPressed(bPressed);
            bPressed = ((pInputBuffer[1] & 0x0008) != 0);
            if (Button3.IsPressed() != bPressed)
                Button3.SetPressed(bPressed);
            bPressed = ((pInputBuffer[1] & 0x0010) != 0);
            if (Button4.IsPressed() != bPressed)
                Button4.SetPressed(bPressed);
            bPressed = ((pInputBuffer[1] & 0x0020) != 0);
            if (Button5.IsPressed() != bPressed)
                Button5.SetPressed(bPressed);
            bPressed = ((pInputBuffer[1] & 0x0040) != 0);
            if (Button6.IsPressed() != bPressed)
                Button6.SetPressed(bPressed);
            bPressed = ((pInputBuffer[1] & 0x0080) != 0);
            if (Button7.IsPressed() != bPressed)
                Button7.SetPressed(bPressed); */

            break;
        }

        default:
            break;
    }

}
}
