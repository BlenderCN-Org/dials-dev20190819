from __future__ import absolute_import, division, print_function
from six.moves import range
import math

"""radius is taken to be d_star, or 1/resolution limit in units of distance"""

def sphere_volume(radius):
  return (4./3.)*math.pi*math.pow(radius,3)

def vol_of_Ewald_sphere_solid_of_revolution(wavelength):
  """Use Pappus's centroid theorem.  Direct quote from Wikipedia:  The volume of a solid
     of revolution generated by rotating a plane Figure F about an external axis is equal to
     the product of the area of F and the distance traveled by its geometric centroid.

     Thus for the Ewald sphere the Figure F has [area = pi * K^2], and the centroid travels
     [distance = 2 * pi * K] therefore [volume = 2 * pi^2 * K^3]"""
  return 2. * math.pi * math.pi / math.pow(wavelength,3)

def sphere_volume_minus_missing_cone(radius,wavelength):
  """Use Pappus's centroid theorem.  Approach 1 (not used here):  Use integral calculus
     to get analytical formula for the solid of revolution.  Approach 2 (used here).
     Use analytical approach to get the area of the plane Figure F followed by
     computational approach to get the centroid and thus the volume of the solid of
     revolution.

     Consider a plane Figure F in the yz plane.  +z is the beam direction.  Ewald sphere
     of radius K is centered at [z = -K].  Resolution sphere of radius R is centered at
     the origin.  The plane figure F is divided vertically into a left side circular
     segment S bounded by the Resolution sphere, and a right side circular segment D bounded
     by the Ewald sphere.

     The vertical dividing line is at Z-coordinate [z = -V], where V is given by a
     separate calculation.

  """
  K = 1./wavelength
  assert radius <= 2.*K # Can never observe diffraction at less than half-wavelength
  D = areaD(radius,K)
  S = areaS(radius,K)
  areaF = D + S
  V = v_coordinate(radius,K)
  """Now find the centroid by a simplex integration.  The centroid must be
     somewhere on the line segment y=0 between [z = -R] and z=0.
  """
  centroid = simplex_integration(D,S,radius,K,V).centroid
  return areaF * 2. * math.pi * centroid

class simplex_integration:
  """Determine the centroid by simplex integration.  It is integral(z dA)/ total area A,
     where the two-d integral is over all area elements dA.
     Integral(z dA) can be re-expressed as Integral(z h(z) dz) where h(z) is the
     vertical chord length through Figure F at position z."""
  def __init__(self,D,S,R,K,V):
    self.V = V; self.D = D; self.S = S; self.R = R; self.K = K
    total_areaF = D + S
    Nsteps = 100
    dz = R/Nsteps
    weighted_sum = 0.0
    for iz in range(Nsteps):
      z = (2*iz+1)*R/(2.*Nsteps)
      if z > V:  # area element is part of segment S
        chord_length = 2. * math.sqrt(R*R - z*z)
      else: # area element is part of segment D
        chord_length = 2. * math.sqrt(2.*K*z-z*z)
      weighted_sum += z * chord_length * dz
    self.centroid = weighted_sum / total_areaF

def areaD(R,K): # Area of D
  beta = math.asin(R/(2.*K)) # quarter angle
  theta = 4.* beta
  return 0.5 * K * K *(theta - math.sin(theta))

def areaS(R,K): # Area of S
  alpha = math.acos(R/(2.*K)) # half angle
  #print "S half angle",alpha*180./math.pi
  theta = 2.* alpha
  return 0.5 * R * R *(theta - math.sin(theta))

def v_coordinate(R,K):
  # By similar triangles:
  return R * R / (2.0 * K)

def print_table():
  wavelength = 1.0 # Angstrom
  K = 1./wavelength
  print("R     K      Vsph  Vewld     D      S      V    Xroid  Vobs   o/s    S/V  Xroid/R")
  for x in range(1,21):
    radius = 0.1 * x # inverse Angstroms
    Vsph = sphere_volume(radius) # reciprocal space volume out to inverse resolution == radius
    Vewld= vol_of_Ewald_sphere_solid_of_revolution(wavelength) # reciprocal space volume
           # impacted by a complete revolution of the Ewald sphere.
    D = areaD(radius,K)
    S = areaS(radius,K)
    V = v_coordinate(radius,K)
    centroid = simplex_integration(D,S,radius,K,V).centroid
    Vobs2 = sphere_volume_minus_missing_cone(radius,wavelength)# actual reciprocal space volume
           # observed (inside Ewald sphere torus of rotation & inside resolution-limit)
    print("%5.3f %5.3f %6.3f %6.3f %6.3f %6.3f %6.3f %6.3f %6.3f %6.3f %6.3f %6.3f"%(
      radius,1./wavelength,Vsph,Vewld,D,S,V,centroid,Vobs2,Vobs2/Vsph,S/V,centroid/radius))
  print("At the R=2K limit, expected Vewld/Vsph ratio is [3*pi/16]=%6.3f, found %6.3f"%(
    (3.*math.pi/16.),Vewld/Vsph))
  print("At the R=2K limit, expected area D is [pi*K^2]=%6.3f, found %6.3f"%(
    (K*K*math.pi),D))
  print("At the R=0 limit, expected S/V==pi and centroid/R = 4/(3*pi) = 0.4244 [from mathworld.wolfram.com]")

def test_formulae():
  import sys
  from six.moves import cStringIO as StringIO
  F = StringIO()
  sys.stdout = F
  print_table()
  result = F.getvalue()
  sys.stdout = sys.__stdout__
  #print result
  assert result == \
"""R     K      Vsph  Vewld     D      S      V    Xroid  Vobs   o/s    S/V  Xroid/R
0.100 1.000  0.004 19.739  0.001  0.015  0.005  0.043  0.004  1.000  2.942  0.433
0.200 1.000  0.034 19.739  0.005  0.055  0.020  0.088  0.033  0.997  2.742  0.442
0.300 1.000  0.113 19.739  0.018  0.114  0.045  0.135  0.112  0.993  2.544  0.450
0.400 1.000  0.268 19.739  0.042  0.188  0.080  0.183  0.265  0.988  2.347  0.458
0.500 1.000  0.524 19.739  0.082  0.269  0.125  0.233  0.514  0.981  2.152  0.466
0.600 1.000  0.905 19.739  0.140  0.353  0.180  0.284  0.880  0.973  1.960  0.474
0.700 1.000  1.437 19.739  0.220  0.434  0.245  0.337  1.383  0.963  1.771  0.481
0.800 1.000  2.145 19.739  0.324  0.507  0.320  0.390  2.039  0.951  1.585  0.488
0.900 1.000  3.054 19.739  0.455  0.569  0.405  0.445  2.862  0.937  1.404  0.494
1.000 1.000  4.189 19.739  0.614  0.614  0.500  0.500  3.860  0.922  1.228  0.500
1.100 1.000  5.575 19.739  0.802  0.640  0.605  0.556  5.040  0.904  1.058  0.506
1.200 1.000  7.238 19.739  1.018  0.644  0.720  0.613  6.399  0.884  0.895  0.511
1.300 1.000  9.203 19.739  1.262  0.624  0.845  0.669  7.932  0.862  0.739  0.515
1.400 1.000 11.494 19.739  1.531  0.579  0.980  0.726  9.621  0.837  0.591  0.518
1.500 1.000 14.137 19.739  1.820  0.510  1.125  0.781 11.440  0.809  0.453  0.521
1.600 1.000 17.157 19.739  2.123  0.419  1.280  0.836 13.346  0.778  0.327  0.522
1.700 1.000 20.580 19.739  2.430  0.309  1.445  0.887 15.276  0.742  0.214  0.522
1.800 1.000 24.429 19.739  2.726  0.190  1.620  0.935 17.134  0.701  0.117  0.519
1.900 1.000 28.731 19.739  2.984  0.076  1.805  0.976 18.756  0.653  0.042  0.514
2.000 1.000 33.510 19.739  3.142  0.000  2.000  1.000 19.745  0.589  0.000  0.500
At the R=2K limit, expected Vewld/Vsph ratio is [3*pi/16]= 0.589, found  0.589
At the R=2K limit, expected area D is [pi*K^2]= 3.142, found  3.142
At the R=0 limit, expected S/V==pi and centroid/R = 4/(3*pi) = 0.4244 [from mathworld.wolfram.com]
"""

if __name__=="__main__":
  test_formulae()
  print("OK")
