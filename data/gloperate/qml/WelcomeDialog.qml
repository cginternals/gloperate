
import QtQuick 2.0
import gloperate.ui 1.0


/**
*  WelcomeDialog
*
*  Dialog displaying a senseless welcome message.
*/
DockPanel
{
    id: panel

    iconClosed: '0270-cancel-circle.png'
    iconOpen:   '0270-cancel-circle.png'

    ScrollArea
    {
        anchors.fill:      parent
        anchors.topMargin: panel.iconHeight + 2 * Ui.style.panelPadding

        contentHeight: content.height

        Column
        {
            id: content

            anchors.left:    parent.left
            anchors.right:   parent.right
            anchors.top:     parent.top
            anchors.margins: Ui.style.panelPadding

            Label
            {
                id: text

                width: content.width

                text: "
<h3>C++ Locator Library</h3>

<p>
cpplocate is a small C++ library that provides tools for applications to locate themselves, their data assets, as well as dependend modules. It is cross-platform and designed to work on all major platforms such as Linux, Windows, and OS X.
</p>

<p>
Especially in the context of cross-platform applications, locating data assets belonging to an application or library is not an easy task. Depending on the situation, data assets can occupy different locations on a system: in a development tree, data might be located relative to the executable in a build directory, when installed into the system, they might be located in a global directory such as /usr/share or C:\Users...\AppData, while for self-contained installs they might be located relative to the executable. When projects are deployed using software installers or archives, the final location can be controlled by the user installing the software, therefore it cannot be known at build-time. The situation gets even more complicated when an application does not only need access to its own data assets, but depends on other modules, such as dynamic libraries or even plugins, which bring their own data. However, a software should be able to locate its data assets as easy as possible and still be relocatable for typical deployment and installation routines to work as expected.
</p>

<p>
To help resolve these problems, cpplocate provides the following features:
<ul>
<li>One</li>
<li>Two</li>
<li>Three</li>
<li>Four</li>
<li>Five</li>
</ul>
</p>

<p>
Learn more at <a href=\"https://github.com/cginternals/cpplocate\">GitHub</a>.
</p>
        "
            }
        }
    }
}
