//! \file Input.hpp
//! Interface of the Input class.

#ifndef GOSU_INPUT_HPP
#define GOSU_INPUT_HPP

#include <Gosu/Fwd.hpp>
#include <Gosu/Platform.hpp>
#include <Gosu/Buttons.hpp>
#include <Gosu/TR1.hpp>

#include <vector>

namespace Gosu
{
    //! Very lightweight class that identifies a button (keyboard, mouse or other device).
    class Button
    {
        unsigned id_;
        
    public:
        //! For internal use.
        explicit Button(unsigned id) : id_(id) {}
        //! For internal use.
        unsigned id() const { return id_; }

        //! Default constructor; == noButton.
        Button() : id_(noButton) {}

        //! Conversion from ButtonName constants.
        Button(ButtonName name) : id_(name) {}
    };
    
    //! Tests whether two Buttons identify the same physical button.
    inline bool operator==(Button lhs, Button rhs)
    {
        return lhs.id() == rhs.id();
    }
    inline bool operator!=(Button lhs, Button rhs)
    {
        return !(lhs == rhs);
    }
    inline bool operator<(Button lhs, Button rhs)
    {
        return lhs.id() < rhs.id();
    }
    
    //! Struct that saves information about a touch on the surface of a multi-
    //! touch device.
    //! Available even on non-iPhone platforms to make it easier to compile the
    //! same source for multiple platforms.
    struct Touch
    {
        //! Allows for identification of a touch across calls.
        void* id;
        //! Position of a touch on the touch screen.
        float x, y;
    };
    typedef std::vector<Touch> Touches;
    
    //! Manages initialization and shutdown of the input system. Only one Input
    //! instance can exist per application.
    class Input
    {
        struct Impl;
        const GOSU_UNIQUE_PTR<Impl> pimpl;
    #if defined(GOSU_CPP11_ENABLED)
        // explicitly forbid copying and moving
        Input(Input&&) = delete;
        Input& operator=(Input&&) = delete;
        Input(const Input&) = delete;
        Input& operator=(const Input&) = delete;
    #endif

    public:
    #ifdef GOSU_IS_IPHONE
        Input(void* view, float updateInterval);
        void feedTouchEvent(int type, void* touches);
    #else
        Input(void* window);
        bool feedSDLEvent(void* event);
    #endif
        
        ~Input();
        
        //! Returns the character a button usually produces, or 0.
        static wchar_t idToChar(Button btn);
        //! Returns the button that has to be pressed to produce the
        //! given character, or noButton.
        static Button charToId(wchar_t ch);
        
        //! Returns true if a button is currently pressed.
        //! Updated every tick.
        static bool down(Button btn);
        
        //! Returns the horizontal position of the mouse relative to the top
        //! left corner of the window given to Input's constructor.
        double mouseX() const;
        //! See mouseX.
        double mouseY() const;
        
        //! Immediately moves the mouse as far towards the desired position
        //! as possible. x and y are relative to the window, just as in mouseX()
        //! and mouseY(), so (0, 0) is the top left corner of the window..
        void setMousePosition(double x, double y);

        // Undocumented for the moment. Also applies to currentTouches().
        void setMouseFactors(double factorX, double factorY, double offsetX = 0, double offsetY = 0);
        
        //! Currently known touches.
        const Touches& currentTouches() const;
        
        //! Accelerometer positions in all three dimensions (smoothened).
        //! Note: Not really deprecated - but temporarily defunct.
        GOSU_DEPRECATED double accelerometerX() const;
        GOSU_DEPRECATED double accelerometerY() const;
        GOSU_DEPRECATED double accelerometerZ() const;
        
        //! Collects new information about which buttons are pressed, where the
        //! mouse is and calls onButtonUp/onButtonDown, if assigned.
        void update();
        
        //! Assignable events that are called by update. You can bind these to your own functions.
        //! If you use the Window class, it will assign these to its own methods.
        std::tr1::function<void (Button)> onButtonDown, onButtonUp;
        
        //! Assignable events that are called by update. You can bind these to your own functions.
        //! If you use the Window class, it will assign these to its own methods.
        std::tr1::function<void (Touch)> onTouchBegan, onTouchMoved, onTouchEnded, onTouchCancelled;
        
        //! Returns the currently active TextInput instance, or 0.
        TextInput* textInput() const;
        //! Sets the currently active TextInput, or clears it (input = 0).
        void setTextInput(TextInput* input);
    };
}

#endif
