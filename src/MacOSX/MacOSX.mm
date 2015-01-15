/* ************************************************************************** */
/*                                                                            */
/*                                                  &&&&&&       &&&&&&       */
/*    MacOSX.mm                                    &------&     &------&      */
/*                                                  &&-----&   &-----&&       */
/*                                                    &&&&#######&&&&         */
/*                                                       #.......#            */
/*                                                       #.....  #            */
/*    This file is part of the                           #...    #            */
/*    Lums library.                                       #######             */
/*                                                                            */
/* ************************************************************************** */

#import <Cocoa/Cocoa.h>
#import <Lums/OperatingSystem.h>

static std::string  res_path;

namespace lm
{
    const std::string&
    resourcePath()
    {
        if (!res_path.empty())
            return res_path;
        res_path = [[[NSBundle mainBundle] resourcePath] cStringUsingEncoding:NSASCIIStringEncoding];
        if (res_path.empty())
            res_path = ".";
        res_path += "/";
        return res_path;
    }
}