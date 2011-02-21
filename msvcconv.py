#!/usr/bin/env python

import fnmatch
import os
import re
import xml.dom.minidom

parseOutputFile_re = re.compile(r'\$\(OutDir\)([^.]*)\.dll')

def getAllProjectFiles(location):
  projfiles = []
  for root, dirnames, filenames in os.walk(location):
    for filename in fnmatch.filter(filenames, '*.vcxproj'):
        projfiles.append(os.path.join(root, filename))
  return projfiles

class ProjectFile:
  def __init__(self, filename):
    self.__filename = filename
    self.__changesMade = False
  
  def _findProjectFilePropertyGroup(self):
    for propGroup in self.__doc.getElementsByTagName("PropertyGroup"):
      if propGroup.hasAttributes():
        continue
      outDirElts = propGroup.getElementsByTagName("OutDir");
      if outDirElts.length > 0:
        return propGroup
    return None

  def _convertOutputFileToTargetName(self, outputFile):
    match = parseOutputFile_re.search(outputFile)
    if match:
      return match.group(1)
    return None
    

  def _handleLink(self, elt):
    outputFileNode = elt.getElementsByTagName("OutputFile")[0]
    if outputFileNode.childNodes.length < 1:
      print "No children!"
      return None
    outputFileTextNode = outputFileNode.childNodes[0] #getText(outputFileNode.childNodes)
    outputFile = outputFileTextNode.data.strip()
    if outputFile == r"$(TargetPath)":
      return None
    else:
      self.__changesMade = True
      defaultText = self.__doc.createTextNode(r'$(TargetPath)')
      outputFileNode.replaceChild(defaultText, outputFileTextNode)
      if outputFile == "":
        return None
      else:
        return self._convertOutputFileToTargetName(outputFile)

  def _addTargetNameCondition(self, targetName, condition):
    for elt in self.__projFileNode.getElementsByTagName("TargetName"):
        if elt.hasAttributes() and elt.getAttribute("Condition") == condition:
            return
    
    # ok, didn't find it
    self.__changesMade = True
    targetNameNode = self.__doc.createElement("TargetName")
    targetNameNode.setAttribute("Condition", condition)
    targetNameText = self.__doc.createTextNode(targetName)
    targetNameNode.appendChild(targetNameText)
    self.__projFileNode.appendChild(targetNameNode)

  def _handleItemDefinitionGroup(self, itemDefGroup):
    if not itemDefGroup.hasAttribute("Condition"):
      # Just an item definition group we don't handle
      return None
    
    condition = itemDefGroup.getAttribute('Condition')
    targetName = self._handleLink(itemDefGroup.getElementsByTagName("Link")[0])
    if targetName is not None:
      #print "%s - %s" % (targetName, condition)
      return (condition, targetName)
    else:
      #print "Already fixed: %s" % condition
      return None
  
  def parseAndFix(self):
    self.__doc = xml.dom.minidom.parse(self.__filename)
    configs = []
    for itemDefGroup in self.__doc.getElementsByTagName("ItemDefinitionGroup"):
      pair = self._handleItemDefinitionGroup(itemDefGroup)
      if pair is not None:
        configs.append(pair)
    self.__projFileNode = self._findProjectFilePropertyGroup()
    if self.__projFileNode is None:
      print "Should not happen: Couldn't find OutDir node to signal where to specify TargetName!"
      os.exit(1)
    
    for condition, targetName in configs:
      self._addTargetNameCondition(targetName, condition)

  def getChangesMade(self):
    return self.__changesMade

  def write(self):
    with open(self.__filename, "w") as fixedProjFile:
      self.__doc.writexml(fixedProjFile)

  def toxml(self):
    return self.__doc.toxml()

if __name__ == "__main__":
  projfiles = getAllProjectFiles("vc9")
  for projfile in projfiles:
    print "\n", projfile
    project = ProjectFile(projfile)
    project.parseAndFix()
    if project.getChangesMade():
      print "Changes were made - writing out fixed version!"
      project.write()

