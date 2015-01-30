/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*    Core.cpp                                       oooooo       oooooo      */
/*                                                 oooooooooo   oooooooooo    */
/*                                                         o%%%%%o            */
/*                                                         %:::::%            */
/*                                                        %:::::::%           */
/*    This file is part of the                             %:::::%            */
/*    Lums library.                                         %%%%%             */
/*                                                                            */
/* ************************************************************************** */

#include <Lums/Core.hpp>
#include <Lums/GL.hpp>

using namespace lm;

Core::Core(int w, int h, const char* name, bool fullscreen)
: _win(w, h, name)
, _it(0)
, _width(w)
, _height(h)
{
    _singleton = this;
}

void
Core::start()
{
    // The renderer created time, and we must consume this time
    // in discrete chunks. That allow us to run the game
    // at max FPS : unlimited when there is no VSync, or
    // 60 on an 60Hz monitor...
    // Ticks are still fixed, so we get the best of both worlds
    
    using namespace std::chrono;
    
    // We avoid using floating point for performance reasons, so
    // we store time as nanoseconds.
    
    typedef unsigned long long      ns;
    typedef high_resolution_clock   localClock;
    typedef localClock::time_point  localTime;
    
    constexpr const ns  delta = 10000000; // 10ms as ns
    ns                  acc = 0;
    ns                  frameDuration;
    localTime           currentTime = localClock::now();
    localTime           newTime;
    
    _running = true;
    while (_running)
    {
        newTime = localClock::now();
        frameDuration = duration_cast<nanoseconds>(newTime - currentTime).count();
        currentTime = newTime;
        
        acc += frameDuration;
        while (acc >= delta)
        {
            acc -= delta;
            doEvent();
            doUpdate();
        }
        doRender();
        std::this_thread::sleep_for(microseconds(400)); // CPU is happy
    }
}

void
Core::pop()
{
    if (_stack.empty())
        return;
    if (_it)
        _it--;
    else
        _jmp = true;
    _stack.front()->unload();
    _stack.erase(_stack.begin());
}

void
Core::remove(GameState* state)
{
    size_t idx = std::find_if(_stack.begin(), _stack.end(), [&](std::unique_ptr<GameState>& p){ return p.get() == state; }) - _stack.begin();
    
    if (idx == _stack.size())
        return;
    if (idx == _it)
        _jmp = true;
    else if (idx < _it)
        _it--;
    _stack[idx]->unload();
    _stack.erase(_stack.begin() + idx);
}

void
Core::clear()
{
    while (!stateless())
        pop();
}

bool
Core::stateless() const
{
    return _stack.empty();
}

void
Core::stop()
{
    _running = false;
}


Core::~Core()
{
    _singleton = nullptr;
}


// PRIVATE


void
Core::doEvent()
{
    GameState*  st;
    Event       event;
    bool        c;
    
    do
    {
        _win.pollEvent(event);
        for (_it = 0; _it < _stack.size(); _it++)
        {
        redo:
            _jmp = false;
            st = _stack[_it].get();
            c = st->forwardEvent(event);
            st->handleEvent(event);
            if (_jmp)
                goto redo;
        }
    }
    while (event.type != Event::Type::None);
}

void
Core::doUpdate()
{
    GameState*  st;
    bool        c;
    
    for (_it = 0; _it < _stack.size(); _it++)
    {
    redo:
        _jmp = false;
        st = _stack[_it].get();
        c = st->forwardUpdate();
        st->update();
        if (_jmp)
            goto redo;
    }
}

void
Core::doRender()
{
    size_t min;

    for (min = 0; min < _stack.size() - 1; min++)
    {
        if (!_stack[min]->forwardRender())
            break;
    }
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    while (min >= 0)
        _stack[min--]->render();
    _win.swap();
}

Core*
Core::_singleton = nullptr;
