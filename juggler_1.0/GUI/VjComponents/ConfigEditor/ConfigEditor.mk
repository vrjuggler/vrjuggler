# You should have received a copy of the GNU Library General Public
# License along with this library; if not, write to the
# Free Software Foundation, Inc., 59 Temple Place - Suite 330,
# Boston, MA 02111-1307, USA.
#
# -----------------------------------------------------------------
# File:          $RCSfile$
# Date modified: $Date$
# Version:       $Revision$
# -----------------------------------------------------------------
#
# *************** <auto-copyright.pl END do not edit this line> ***************

# -----------------------------------------------------------------------------
# This include file <ConfigEditor.mk> defines the list of .java files that 
# are built into Java bytecode with javac.
# -----------------------------------------------------------------------------
# Variables defined here are:
#
# VJCCONF_SOURCES - The list of .java files to build
# VJCCONF_OBJS    - The list of .class files that will be created.
# -----------------------------------------------------------------------------

VJCCONF_SOURCES = VjComponents/ConfigEditor/ChunkDBPanel.java		\
                  VjComponents/ConfigEditor/ChunkDBTreeCellRenderer.java \
                  VjComponents/ConfigEditor/ChunkDBTreeModel.java	\
                  VjComponents/ConfigEditor/ChunkDBTreeModelFactory.java \
                  VjComponents/ConfigEditor/ChunkOrgTree.java		\
                  VjComponents/ConfigEditor/ChunkOrgTreeModule.java	\
                  VjComponents/ConfigEditor/ChunkOrgTreePane.java	\
                  VjComponents/ConfigEditor/ChunkTreeNodeInfo.java	\
                  VjComponents/ConfigEditor/ConfigChunkPanel.java	\
                  VjComponents/ConfigEditor/ConfigCommunicator.java	\
                  VjComponents/ConfigEditor/ConfigModule.java		\
                  VjComponents/ConfigEditor/ConfigModuleEvent.java	\
                  VjComponents/ConfigEditor/ConfigModuleListener.java	\
                  VjComponents/ConfigEditor/ConfigSaveMonitorModule.java \
                  VjComponents/ConfigEditor/ConfigUIHelper.java		\
                  VjComponents/ConfigEditor/ConfigurePane.java		\
                  VjComponents/ConfigEditor/DependencyFrame.java	\
                  VjComponents/ConfigEditor/DescDBPanel.java		\
                  VjComponents/ConfigEditor/OrgTreeElem.java

VJCCONF_OBJS = $(VJCCONF_SOURCES:.java=.class)
