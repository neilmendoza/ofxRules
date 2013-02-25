/*
 *  Generator.cpp
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
#include "Generator.h"
#include "ofxXmlSettings.h"
#include "LineAction.h"
#include "TransformAction.h"
#include "PointAction.h"
#include "CubeAction.h"

namespace itg
{
    Generator::Generator() :
        maxDepth(numeric_limits<unsigned>::max())
    {
        registerAction<LineAction>("line");
        registerAction<PointAction>("point");
        registerAction<TransformAction>("transform");
        registerAction<CubeAction>("cube");
       
        mesh.setUsage(GL_DYNAMIC_DRAW);
        mesh.setMode(OF_PRIMITIVE_TRIANGLES);
    }
    
    
    void Generator::step()
    {
        step(mesh);
    }
    
    void Generator::step(ofMesh& mesh)
    {
        list<Branch::Ptr> newBranches;
        
        for (list<Branch::Ptr>::iterator it = branches.begin(); it != branches.end(); ++it)
        {
            if ((*it)->getDepth() < maxDepth && !(*it)->getNextRuleName().empty())
            {
                RuleSet::Ptr ruleSet = ruleSets[(*it)->getNextRuleName()];
                
                vector<Branch::Ptr> children = ruleSet->randomRule()->step(*it, mesh);
                
                newBranches.insert(newBranches.end(), children.begin(), children.end());
            }
        }
        
        //if (newBranches.size() > 10) newBranches.pop_front();
        
        branches = newBranches;
    }
    
    void Generator::draw()
    {
        mesh.draw();
    }
    
    void Generator::load(const string& fileName)
    {
        ofxXmlSettings xml;
        xml.loadFile(fileName);
        setMaxDepth(xml.getAttribute("rules", "maxDepth", (int)numeric_limits<unsigned>::max()));
        string primitive = xml.getAttribute("rules", "primitive", "");
        if (!primitive.empty())
        {
            if (primitive == "triangles") mesh.setMode(OF_PRIMITIVE_TRIANGLES);
            else if (primitive == "triangle_strip") mesh.setMode(OF_PRIMITIVE_TRIANGLE_STRIP);
            else if (primitive == "triangle_fan") mesh.setMode(OF_PRIMITIVE_TRIANGLE_FAN);
            else if (primitive == "lines") mesh.setMode(OF_PRIMITIVE_LINES);
            else if (primitive == "line_strip") mesh.setMode(OF_PRIMITIVE_LINE_STRIP);
            else if (primitive == "line_loop") mesh.setMode(OF_PRIMITIVE_LINE_LOOP);
            else if (primitive == "points") mesh.setMode(OF_PRIMITIVE_POINTS);
        }
        xml.pushTag("rules");
        for (unsigned i = 0; i < xml.getNumTags("ruleSet"); ++i)
        {
            string name = xml.getAttribute("ruleSet", "name", "", i);
            xml.pushTag("ruleSet", i);
            
            for (unsigned j = 0; j < xml.getNumTags("rule"); ++j)
            {
                Rule::Ptr rule = addRule(name, xml.getAttribute("rule", "weight", 0.f, j));
                xml.pushTag("rule", j);
                
                for (CreatorIt it = creators.begin(); it != creators.end(); ++it)
                {
                    for (unsigned k = 0; k < xml.getNumTags(it->first); ++k)
                    {
                        Action::Ptr action = (this->*it->second)();
                        action->load(xml, it->first, k);
                        rule->addAction(action);
                    }
                }
                xml.popTag();
            }
            xml.popTag();
        }
    }

    void Generator::watchFile(const string& watchedFileName, bool autoCheck, float checkPeriod)
    {
        this->watchedFileName = watchedFileName;
        watchedLastModified = Poco::Timestamp(0);
        if (autoCheck)
        {
            lastChecked = 0.f;
            this->checkPeriod = checkPeriod;
            ofAddListener(ofEvents().update, this, &Generator::onUpdate);
        }
    }

    void Generator::onUpdate(ofEventArgs& args)
    {
        if (ofGetElapsedTimef() - lastChecked > checkPeriod)
        {
            checkWatchedFile();
            lastChecked = ofGetElapsedTimef();
        }
    }
    
    void Generator::checkWatchedFile()
    {
        Poco::Timestamp timestamp = ofFile(watchedFileName).getPocoFile().getLastModified();
        if (timestamp > watchedLastModified)
        {
            mesh.clear();
            load(watchedFileName);
            branches.clear();
            addBranch("test");
            watchedLastModified = timestamp;
        }
    }
    
    Rule::Ptr Generator::addRule(const string& ruleName, float weight)
    {
        if (!ruleSets[ruleName]) ruleSets[ruleName] = RuleSet::Ptr(new RuleSet(ruleName));
        ruleSets[ruleName]->addRule(Rule::Ptr(new Rule(weight)));
        return ruleSets[ruleName]->back();
    }
    
    Branch::Ptr Generator::addBranch(const string& ruleName, const ofMatrix4x4& transform)
    {
        Branch::Ptr branch = Branch::Ptr(new Branch(ruleName, 0, transform));
        branches.push_back(branch);
        return branch;
    }
}
