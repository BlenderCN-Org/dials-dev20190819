from __future__ import absolute_import, division, print_function
from six.moves import range
import os
import h5py
import numpy as np
from xfel.euxfel.read_geom import read_geom
from libtbx.phil import parse
import six

phil_scope = parse("""
  unassembled_file = None
    .type = str
    .help = cheetah file used to read in image data(.cxi).
  geom_file = None
    .type = str
    .help = geometry file to be read in for detector (.geom).
  detector_distance = None
    .type = float
    .help = Detector distance
  wavelength = None
    .type = float
    .help = If not provided, try to find wavelength in unassembled file.
  mask_file = None
    .type = str
    .help = Path to file with external bad pixel mask.
  split_modules_into_asics = True
    .type = bool
    .help = Whether to split the 4x2 modules into indivdual asics \
            accounting for borders and gaps.
  trusted_range = None
    .type = floats(size=2)
    .help = Set the trusted range
""")


'''

This script creates a master nexus file by taking in as input a) a .cxi file and b) a .geom file
The cxi file is generated by cheetah after processing the raw images and doing appropriate gain corrections
The assumed parameters for the detector can be seen in the __init__ function and should be changed
if they are modified at in the future

'''


class jf16m_cxigeom2nexus(object):
  def __init__(self, args):
    self.params_from_phil(args)
    if self.params.detector_distance == None:
      self.params.detector_distance = 100.0 # Set detector distance arbitrarily if nothing is provided
    self.hierarchy = read_geom(self.params.geom_file)
    self.n_quads = 4
    self.n_modules = 8

  def params_from_phil(self, args):
    user_phil = []
    for arg in args:
      if os.path.isfile(arg):
        user_phil.append(parse(file_name=arg))
      else:
        try:
          user_phil.append(parse(arg))
        except Exception as e:
          raise Sorry("Unrecognized argument: %s"%arg)
    self.params = phil_scope.fetch(sources=user_phil).extract()

  def _create_scalar(self, handle,path,dtype,value):
    dataset = handle.create_dataset(path, (),dtype=dtype)
    dataset[()] = value

  def create_vector(self,handle, name, value, **attributes):
    handle.create_dataset(name, (1,), data = [value], dtype='f')
    for key,attribute in six.iteritems(attributes):
      handle[name].attrs[key] = attribute

  def create_nexus_master_file(self):

    '''
    Hierarchical structure of master nexus file. Format information available here
    http://download.nexusformat.org/sphinx/classes/base_classes/NXdetector_module.html#nxdetector-module
    --> entry
      --> data
      --> definition (leaf)
      --> instrument
      --> sample
    '''
    output_file_name = os.path.splitext(self.params.unassembled_file)[0]+'_master.h5'
    f = h5py.File(output_file_name, 'w')
    entry = f.create_group('entry')
    entry.attrs['NX_class'] = 'NXentry'
    # --> definition
    self._create_scalar(entry, 'definition', 'S4', 'NXmx')
    # --> data
    data = entry.create_group('data')
    data.attrs['NX_class'] = 'NXdata'
    data_key = 'data'
    data[data_key] = h5py.ExternalLink(self.params.unassembled_file, "data/data")
    # --> sample
    sample = entry.create_group('sample')
    sample.attrs['NX_class'] = 'NXsample'
    beam = sample.create_group('beam')
    beam.attrs['NX_class'] = 'NXbeam'
    if self.params.wavelength is None:
      wavelengths = h5py.File(self.params.unassembled_file, 'r')['instrument/photon_wavelength_A']
      beam.create_dataset('incident_wavelength', (1,), data=np.mean(wavelengths), dtype='f8')
    else:
      beam.create_dataset('incident_wavelength', (1,), data=self.params.wavelength, dtype='f8')
    beam['incident_wavelength'].attrs['units'] = 'angstrom'
    # --> instrument
    instrument = entry.create_group('instrument')
    instrument.attrs['NX_class'] = 'NXinstrument'
    jf16m = instrument.create_group('JF16M')
    jf16m.attrs['NX_class'] = 'NXdetector_group'
    jf16m.create_dataset('group_index', data = list(range(1,3)), dtype='i')
    data = ['JF16M','ELE_D0']
    jf16m.create_dataset('group_names',(2,), data=data, dtype='S12')
    jf16m.create_dataset('group_parent',(2,), data=[-1,1], dtype='i')
    jf16m.create_dataset('group_type', (2,), data=[1,2], dtype='i')
    transformations = jf16m.create_group('transformations')
    transformations.attrs['NX_class'] = 'NXtransformations'
    # Create AXIS leaves for RAIL, D0 and different hierarchical levels of detector
    self.create_vector(transformations, 'AXIS_RAIL', self.params.detector_distance, depends_on='.', equipment='detector', equipment_component='detector_arm',transformation_type='translation', units='mm', vector=(0., 0., 1.))
    self.create_vector(transformations, 'AXIS_D0', 0.0, depends_on='AXIS_RAIL', equipment='detector', equipment_component='detector_arm',transformation_type='rotation', units='degrees', vector=(0., 0., -1.), offset=self.hierarchy.local_origin, offset_units = 'mm')
    # Add 4 quadrants
    # Nexus coordiate system, into the board         JF16M detector
    #      o --------> (+x)                             Q3=(12,13,14,15) Q0=(0,1,2,3)
    #      |                                                        o
    #      |                                            Q2=(8,9,10,11)   Q1=(4,5,6,7)
    #      v
    #     (+y)

    panels = []
    for q, quad in six.iteritems(self.hierarchy):
      panels.extend([quad[key] for key in quad])
    fast = max([int(panel['max_fs']) for panel in panels])+1
    slow = max([int(panel['max_ss']) for panel in panels])+1
    pixel_size = panels[0]['pixel_size']
    assert [pixel_size == panels[i+1]['pixel_size'] for i in range(len(panels)-1)].count(False) == 0

    quad_fast = fast
    quad_slow = slow // self.n_quads
    module_fast = quad_fast
    module_slow = quad_slow // self.n_modules

    if self.params.split_modules_into_asics:
      n_fast_asics = 4; n_slow_asics = 2
      border = 1; gap = 2
      asic_fast = (module_fast - (n_fast_asics-1)*gap) / n_fast_asics # includes border but not gap
      asic_slow = (module_slow - (n_slow_asics-1)*gap) / n_slow_asics # includes border but not gap

    detector = instrument.create_group('ELE_D0')
    detector.attrs['NX_class']  = 'NXdetector'
    array_name = 'ARRAY_D0'

    if self.params.mask_file is not None:
      detector.create_dataset('pixel_mask_applied', (1,), data=[True], dtype='uint32')
      #detector['pixel_mask'] = h5py.ExternalLink(self.params.mask_file, "mask") # If mask was formatted right, could use this
      mask = h5py.File(self.params.mask_file, 'r')['mask'][()].astype(np.int32)
      detector.create_dataset('pixel_mask', mask.shape, data=mask==0, dtype=mask.dtype)

    if self.params.trusted_range is not None:
      underload, overload = self.params.trusted_range
      detector.create_dataset('underload_value', (1,), data=[underload], dtype='int32')
      detector.create_dataset('saturation_value', (1,), data=[overload], dtype='int32')

    alias = 'data'
    data_name = 'data'
    detector[alias] = h5py.SoftLink('/entry/data/%s'%data_name)

    for q_key in sorted(self.hierarchy.keys()):
      quad = int(q_key.lstrip('quad'))
      q_name = 'AXIS_D0Q%d'%quad
      quad_vector = self.hierarchy[q_key].local_origin.elems
      self.create_vector(transformations, q_name, 0.0, depends_on='AXIS_D0', equipment='detector', equipment_component='detector_quad',transformation_type='rotation', units='degrees', vector=(0., 0., -1.), offset = quad_vector, offset_units = 'mm')
      for m_key in sorted(self.hierarchy[q_key].keys()):
        module_num = int(m_key.lstrip('m'))
        m_name = 'AXIS_D0Q%dM%d'%(quad, module_num)
        module_vector = self.hierarchy[q_key][m_key]['local_origin']
        fast = self.hierarchy[q_key][m_key]['local_fast']
        slow = self.hierarchy[q_key][m_key]['local_slow']
        if self.params.split_modules_into_asics:
          # module_vector points to the corner of the module. Instead, point to the center of it.
          offset = (module_fast/2 * pixel_size * fast) + (module_slow/2 * pixel_size * slow)
          module_vector = module_vector + offset
          self.create_vector(transformations, m_name, 0.0, depends_on=q_name, equipment='detector', equipment_component='detector_module',transformation_type='rotation', units='degrees', vector=(0., 0., -1.), offset = module_vector.elems, offset_units = 'mm')

          for asic_fast_number in range(n_fast_asics):
            for asic_slow_number in range(n_slow_asics):
              asic_num = asic_fast_number + (asic_slow_number * n_fast_asics)
              a_name = 'AXIS_D0Q%dM%dA%d'%(quad, module_num, asic_num)
              # Modules look like this:
              # bbbbbggbbbbb Assuming a 3x3 asic (X), a 1 pixel border (b) and a 2 pixel gap (g).
              # bXXXbggbXXXb This is as if there were only two asics in a module
              # bXXXbggbXXXb The math below skips past the borders and gaps to the first real pixel.
              # bXXXbggbXXXb
              # bbbbbggbbbbb
              asic_vector = -offset + (fast * pixel_size * (asic_fast * asic_fast_number + border + (asic_fast_number * gap))) + \
                                      (slow * pixel_size * (asic_slow * asic_slow_number + border + (asic_slow_number * gap)))
              self.create_vector(transformations, a_name, 0.0, depends_on=m_name, equipment='detector', equipment_component='detector_asic',
                                 transformation_type='rotation', units='degrees', vector=(0., 0., -1.), offset = asic_vector.elems, offset_units = 'mm')

              asicmodule = detector.create_group(array_name+'Q%dM%dA%d'%(quad,module_num,asic_num))
              asicmodule.attrs['NX_class'] = 'NXdetector_module'
              asicmodule.create_dataset('data_origin', (2,), data=[module_slow * module_num + asic_slow * asic_slow_number + border + gap*asic_slow_number,
                                                                                              asic_fast * asic_fast_number + border + gap*asic_fast_number], dtype='i')
              asicmodule.create_dataset('data_size', (2,), data=[asic_slow - border*2, asic_fast - border*2], dtype='i')

              self.create_vector(asicmodule, 'fast_pixel_direction',pixel_size,
                                 depends_on=transformations.name+'/AXIS_D0Q%dM%dA%d'%(quad,module_num,asic_num),
                                 transformation_type='translation', units='mm', vector=fast.elems, offset=(0. ,0., 0.))
              self.create_vector(asicmodule, 'slow_pixel_direction',pixel_size,
                                 depends_on=transformations.name+'/AXIS_D0Q%dM%dA%d'%(quad,module_num,asic_num),
                                 transformation_type='translation', units='mm', vector=slow.elems, offset=(0., 0., 0.))
        else:
          module_vector = self.hierarchy[q_key][m_key]['local_origin'].elems
          self.create_vector(transformations, m_name, 0.0, depends_on=q_name,
                             equipment='detector', equipment_component='detector_module',
                             transformation_type='rotation', units='degrees', vector=(0. ,0., -1.),
                             offset = module_vector, offset_units = 'mm')

          modulemodule = detector.create_group(array_name+'Q%dM%d'%(quad,module_num))
          modulemodule.attrs['NX_class'] = 'NXdetector_module'
          modulemodule.create_dataset('data_origin', (2,), data=[module_slow * module_num, 0],
                                                           dtype='i')
          modulemodule.create_dataset('data_size', (2,), data=[module_slow, module_fast], dtype='i')

          fast = self.hierarchy[q_key][m_key]['local_fast'].elems
          slow = self.hierarchy[q_key][m_key]['local_slow'].elems
          self.create_vector(modulemodule, 'fast_pixel_direction',pixel_size,
                             depends_on=transformations.name+'/AXIS_D0Q%dM%d'%(quad,module_num),
                             transformation_type='translation', units='mm', vector=fast, offset=(0. ,0., 0.))
          self.create_vector(modulemodule, 'slow_pixel_direction',pixel_size,
                             depends_on=transformations.name+'/AXIS_D0Q%dM%d'%(quad,module_num),
                             transformation_type='translation', units='mm', vector=slow, offset=(0., 0., 0.))
    f.close()

if __name__ == '__main__':
  import sys
  nexus_helper = jf16m_cxigeom2nexus(sys.argv[1:])
  nexus_helper.create_nexus_master_file()