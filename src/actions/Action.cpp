/*
 *  Action.cpp
 *
 *  Copyright (c) 2013, Neil Mendoza, http://www.neilmendoza.com
 *  All rights reserved. 
 *  
 *  Redistribution and use in source and binary forms, with or without 
 *  modification, are permitted provided that the following conditions are met: 
 *  
 *  * Redistributions of source code must retain the above copyright notice, 
 *    this list of conditions and the following disclaimer. 
 *  * Redistributions in binary form must reproduce the above copyright 
 *    notice, this list of conditions and the following disclaimer in the 
 *    documentation and/or other materials provided with the distribution. 
 *  * Neither the name of Neil Mendoza nor the names of its contributors may be used 
 *    to endorse or promote products derived from this software without 
 *    specific prior written permission. 
 *  
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
 *  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE 
 *  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE 
 *  ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE 
 *  LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR 
 *  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF 
 *  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
 *  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
 *  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE 
 *  POSSIBILITY OF SUCH DAMAGE. 
 *
 */
#include "Action.h"

namespace itg
{
    Action::Action(const string& nextRuleName) : nextRuleName(nextRuleName)
    {
    }
    
    ofMatrix4x4 Action::inverseTranspose(const ofMatrix4x4& transform) const
    {
        ofMatrix4x4 normalMatrix = ofMatrix4x4::getTransposedOf(const_cast<ofMatrix4x4&>(transform).getInverse());
        return ofMatrix4x4(normalMatrix(0, 0), normalMatrix(0, 1), normalMatrix(0, 2), 0.f,
                           normalMatrix(1, 0), normalMatrix(1, 1), normalMatrix(1, 2), 0.f,
                           normalMatrix(2, 0), normalMatrix(2, 1), normalMatrix(2, 2), 0.f,
                           0.f,                0.f,                0.f,                1.f);
    }
    
    void Action::load(ofxXmlSettings& xml, const string& tagName, unsigned tagNum)
    {
        ofLogError() << "Undefined action load function called";
    }
    
    void Action::save(ofxXmlSettings& xml)
    {
        ofLogError() << "Undefined action save function called";
    }
    
    ofFloatColor Action::parseColour(const string& colourString)
    {
        vector<string> split = ofSplitString(colourString, " ");
        ofFloatColor colour;
        if (split.empty()) return ofFloatColor(1.f, 1.f, 1.f);
        else if (split.size() != 3) ofLogError() << "Colour format error";
        else
        {
            if (colourString.find('.') == string::npos)
            {
                colour.set(atoi(split[0].c_str()) / 255.f, atoi(split[1].c_str()) / 255.f, atoi(split[2].c_str()) / 255.f);
            }
            else
            {
                colour.set(atof(split[0].c_str()), atof(split[1].c_str()), atof(split[2].c_str()));
            }
        }
        return colour;
    }
}
