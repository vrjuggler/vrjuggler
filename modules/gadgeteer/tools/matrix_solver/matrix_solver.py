"""
Matrix Solver
Parses a calibration table and solves the equations for the alpha constants
used in the Hardy's Multi-Quadric method of calibration.
"""
import os, sys, string
from math import sqrt
from xml.dom import *
from xml.dom.minidom import *
import Numeric, LinearAlgebra

# Define useful functions
def length(v):
   """
   Determines the magnitude of a three dimensional vector, v.
   """
   return sqrt( v[0] * v[0] + v[1] * v[1] + v[2] * v[2] )

def vec_subtract(a, b):
   """
   Returns a tuple c, s.t. c = a - b
   """
   return (a[0] - b[0], a[1] - b[1], a[2] - b[2])
   
def vec_multiply(a, b):
   """
   Returns the scalar result of a dot b.
   """
   return a[0] * b[0] + a[1] * b[1] + a[2] * b[2]

argc = len(sys.argv)
if argc < 2 or argc > 3:
   print "Usage:  matrix_solver.py input_file [output_file]"
   sys.exit(1)

# XXX: Take out the debug file when ready.
dbg_file = file('debug_output.txt', 'w')
# Open the table file
in_file = file(sys.argv[1], 'r')
doc = parse(in_file)
root_element = doc.documentElement
# Get the offsets from the table
offset_elements = root_element.getElementsByTagName('Offset')
offset_table = {}
# This has to be done since keys and values in Python dictionaries are stored
# in random order.
keys_in_order = []
dbg_file.write('Parsed Offsets\n')
# Build an offset table.
for e in offset_elements:
   curr_offset = string.split(e.firstChild.data)
   qx = e.attributes['X'].nodeValue
   qy = e.attributes['Y'].nodeValue
   qz = e.attributes['Z'].nodeValue
   q = ( float(qx), float(qy), float(qz) )
   px = curr_offset[0]
   py = curr_offset[1]
   pz = curr_offset[2]
   p = ( float(px), float(py), float(pz) )
   dbg_file.write('(' + qx + ',' + qy + ',' + qz + '):(' + px + ',' + py + ',' + pz + ')\n')
   dbg_file.write(str(q) + ' : ' + str(p) + '\n')
   offset_table[q] = p
   keys_in_order.append(q)
dbg_file.write('\nOffset Table\n')
dbg_file.write(str(offset_table))
# w[j](p) = sqrt( (p-p[j]) * (p-p[j]) + R^2 )
# s.t. 10 <= pow(R, 2) <= 1000
w_matrix_list = []
r_squared = 0.4
print 'Calculating W Matrix...'
for i in range(0, len(offset_table)):
   w_matrix_row = []
   p = offset_table[keys_in_order[i]]
   for j in range(0, len(offset_table)):
      pj = offset_table[keys_in_order[j]]
      p_difference = vec_subtract(p, pj)
      w = sqrt(vec_multiply(p_difference, p_difference) + r_squared)
      w_matrix_row.append(w)
   w_matrix_list.append(w_matrix_row)
dbg_file.write('\nW Matrix List\n')
dbg_file.write( str(w_matrix_list) )
w_matrix = Numeric.array(w_matrix_list)
dbg_file.write('\nW Matrix\n')
dbg_file.write( str(w_matrix) )
q_list = []
#for q in offset_table.values():
#   q_list.append(list(q))
for k in keys_in_order:
   q_list.append( list(k) )
dbg_file.write('\nQ List\n')
dbg_file.write( str(q_list) )
q_vector = Numeric.array(q_list)
print 'Solving for alpha vector...'
alpha_vector = LinearAlgebra.solve_linear_equations(w_matrix, q_vector)
dbg_file.write('\nAlpha Vector\n')
dbg_file.write( str(alpha_vector) )
print 'Alpha Vector found.'
out_file = ''
if argc == '2':
   out_file = sys.argv[1]
else:
   out_file = sys.argv[2]
in_file.close()
out_file = file(out_file, 'w')
alpha_vector_list = alpha_vector.tolist()
dbg_file.write('\nCheck Solution\n')
solution_check = Numeric.matrixmultiply(w_matrix, alpha_vector)
dbg_file.write( str(solution_check) )

# Add Alpha constants to XML Tree
for i in alpha_vector_list:
   element = Element('Alpha')
   element.setAttribute('X', str(i[0]))
   element.setAttribute('Y', str(i[1]))
   element.setAttribute('Z', str(i[2]))
   root_element.appendChild(element)
out_file.write(doc.toprettyxml())
out_file.close()
