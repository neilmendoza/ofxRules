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

namespace itg
{
    
    Generator::Generator() : maxDepth(numeric_limits<unsigned>::max())
    {
        registerAction<LineAction>("line");
        registerAction<PointAction>("point");
        registerAction<TransformAction>("transform");
    }
    
    void Generator::load(const string& fileName)
    {
        ofxXmlSettings xml;
        xml.loadFile(fileName);
        setMaxDepth(xml.getAttribute("rules", "maxDepth", (int)numeric_limits<unsigned>::max()));
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
        
        /*structure.setMaxDepth(300);
        
        {
            Rule::Ptr rule = structure.addRule("test", 100);
            
            LineAction::Ptr action = rule->addAction<LineAction>("test");
            action->translate(0, 0.1, 0);
            action->rotate(1, 0, 0);
        }
        
        {
            Rule::Ptr rule = structure.addRule("test", 100);
            
            LineAction::Ptr action = rule->addAction<LineAction>("test");
            action->translate(0, 0.1, 0);
            action->rotate(0, 20, 0);
        }
        
        {
            Rule::Ptr rule = structure.addRule("test", 100);
            
            LineAction::Ptr action = rule->addAction<LineAction>("test");
            action->translate(0, 0.4, 0);
            action->rotate(1, 0, -2);
        }
        
        {
            Rule::Ptr rule = structure.addRule("test", 6);
            
            TransformAction::Ptr left = rule->addAction<TransformAction>("test");
            left->rotate(0, 180, 0);
            
            TransformAction::Ptr right = rule->addAction<TransformAction>("test");
            left->rotate(15, 0, 0);
        }*/
        

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

    void Generator::step(ofMesh& mesh)
    {
        if (branches.front()->getDepth() < maxDepth)
        {
            list<Branch::Ptr> newBranches;
            
            for (list<Branch::Ptr>::iterator it = branches.begin(); it != branches.end(); ++it)
            {
                RuleSet::Ptr ruleSet = ruleSets[(*it)->getNextRuleName()];
                
                vector<Branch::Ptr> children = ruleSet->randomRule()->step(*it, mesh);
                
                newBranches.insert(newBranches.end(), children.begin(), children.end());
            }
            
            //if (newBranches.size() > 10) newBranches.pop_front();
            
            branches = newBranches;
        }
    }
}
