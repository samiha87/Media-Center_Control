import QtQuick 2.11
import QtQuick.Controls 2.1
// Volume button with animation
Button {
    id: volumeDownButton
    width: 50
    height: 50
    background: Image {
        id:volumeDownButtonImage
        anchors.fill: parent
        fillMode: Image.PreserveAspectFit
        source: "qrc:/icons/Arrow/arrow_animation_1.png"
    }

    onPressed: {
        volumeDownButtonAnimationPressed.visible = true
        volumeDownButtonAnimationPressed.playing = true
        volumeDownButtonImage.visible = false;
    }
    onReleased: {
        volumeDownButtonAnimationPressed.visible = false
        volumeDownButtonAnimationPressed.playing = false

        volumeDownButtonAnimationReleased.visible = true
        volumeDownButtonAnimationReleased.playing = true
        volumeDownButtonImage.visible = false;
    }

    AnimatedImage {
        playing: false
        visible: false
        id:volumeDownButtonAnimationPressed
        rotation: 0
        width: parent.width
        height: parent.height
        source: "qrc:/icons/Arrow/arrow_animation_pressed.gif"
        speed: 10.0
        onFrameChanged:{
           console.log(currentFrame)
           if(currentFrame == 15) {
               volumeDownButtonAnimationPressed.visible = false
               volumeDownButtonAnimationPressed.playing = false
               volumeDownButtonImage.visible = true;
               volumeDownButtonImage.source = "qrc:/icons/Arrow/arrow_animation_20.png"
           }
        }
    }

    AnimatedImage {
        playing: false
        visible: false
        id:volumeDownButtonAnimationReleased
        width: parent.width
        height: parent.height
        source: "qrc:/icons/Arrow/arrow_animation_released.gif"
        speed: 10.0
        onFrameChanged:{
           console.log(currentFrame)
           if(currentFrame == 12) {
               volumeDownButtonAnimationReleased.visible = false
               volumeDownButtonAnimationReleased.playing = false
               volumeDownButtonImage.visible = true
               volumeDownButtonImage.source = "qrc:/icons/Arrow/arrow_animation_1.png"
           }

        }
    }
}
