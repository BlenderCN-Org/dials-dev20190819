# coding: utf-8
from __future__ import absolute_import, division, print_function

from builtins import range
import os

from scitbx.array_family import flex

from dxtbx.format.Format import Format
from dxtbx.model import MultiAxisGoniometer


class Reader(object):

    _format_class_ = None

    def __init__(self, filenames, num_images=None, **kwargs):
        self.kwargs = kwargs
        self.format_class = Reader._format_class_
        assert len(filenames) == 1
        self._filename = filenames[0]
        if num_images is None:
            self._num_images = self.read_num_images()
        else:
            self._num_images = num_images

    def nullify_format_instance(self):
        self.format_class._current_instance_ = None
        self.format_class._current_filename_ = None

    def read(self, index):
        format_instance = self.format_class.get_instance(self._filename, **self.kwargs)
        return format_instance.get_raw_data(index)

    def paths(self):
        return [self._filename]

    def read_num_images(self):
        format_instance = self.format_class.get_instance(self._filename, **self.kwargs)
        return format_instance.get_num_images()

    def num_images(self):
        return self._num_images

    def __len__(self):
        return self.num_images()

    def copy(self, filenames, indices=None):
        return Reader(filenames, indices)

    def identifiers(self):
        return ["%s-%d" % (self._filename, index) for index in range(len(self))]

    def is_single_file_reader(self):
        return True

    def master_path(self):
        return self._filename


class FormatMultiImage(Format):
    def __init__(self, **kwargs):
        pass

    def get_num_images(self):
        raise NotImplementedError

    def get_goniometer(self, index=None):
        return self._goniometer_instance

    def get_detector(self, index=None):
        return self._detector_instance

    def get_beam(self, index=None):
        return self._beam_instance

    def get_scan(self, index=None):
        return self._scan_instance

    def get_raw_data(self, index=None):
        raise NotImplementedError

    def get_mask(self, index=None, goniometer=None):
        return None

    def get_detectorbase(self, index=None):
        raise NotImplementedError

    @classmethod
    def get_reader(cls):
        """
        Return a reader class

        """
        obj = Reader
        obj._format_class_ = cls
        return obj

    def get_masker(self, goniometer=None):
        """
        Return a reader class

        """
        if (
            isinstance(goniometer, MultiAxisGoniometer)
            and hasattr(self, "_dynamic_shadowing")
            and self._dynamic_shadowing
        ):
            masker = self.get_goniometer_shadow_masker(goniometer=goniometer)
        else:
            masker = None
        return masker

    @classmethod
    def get_imageset(
        cls,
        filenames,
        beam=None,
        detector=None,
        goniometer=None,
        scan=None,
        as_sweep=False,
        as_imageset=False,
        single_file_indices=None,
        format_kwargs=None,
        template=None,
        check_format=True,
        lazy=False,
    ):
        """
        Factory method to create an imageset

        """
        from dxtbx.imageset import ImageSetData
        from dxtbx.imageset import ImageSweep

        if isinstance(filenames, str):
            filenames = [filenames]
        elif len(filenames) > 1:
            assert len(set(filenames)) == 1
            filenames = filenames[0:1]

        # Make filenames absolute
        filenames = [os.path.abspath(x) for x in filenames]

        # Make it a dictionary
        if format_kwargs is None:
            format_kwargs = {}

        # If get_num_images hasn't been implemented, we need indices for number of images
        if cls.get_num_images == FormatMultiImage.get_num_images:
            assert single_file_indices is not None
            assert min(single_file_indices) >= 0
            num_images = max(single_file_indices) + 1
        else:
            num_images = None

        # Get some information from the format class
        reader = cls.get_reader()(filenames, num_images=num_images, **format_kwargs)

        # Get the format instance
        assert len(filenames) == 1
        if check_format is True:
            format_instance = cls(filenames[0], **format_kwargs)
        else:
            format_instance = None
            if not as_sweep:
                lazy = True

        # Read the vendor type
        if check_format is True:
            vendor = format_instance.get_vendortype()
        else:
            vendor = ""

        # Get the format kwargs
        params = format_kwargs

        # Check if we have a sweep

        # Make sure only 1 or none is set
        assert [as_imageset, as_sweep].count(True) < 2
        if as_imageset:
            is_sweep = False
        elif as_sweep:
            is_sweep = True
        else:
            if scan is None and format_instance is None:
                raise RuntimeError(
                    """
          One of the following needs to be set
            - as_imageset=True
            - as_sweep=True
            - scan
            - check_format=True
      """
                )
            if scan is None:
                test_scan = format_instance.get_scan()
            else:
                test_scan = scan
            if test_scan is not None and test_scan.get_oscillation()[1] != 0:
                is_sweep = True
            else:
                is_sweep = False

        assert not (as_sweep and lazy), "No lazy support for sweeps"

        if single_file_indices is not None:
            single_file_indices = flex.size_t(single_file_indices)

        # Create an imageset or sweep
        if not is_sweep:

            # Use imagesetlazy
            # Setup ImageSetLazy and just return it. No models are set.
            if lazy:
                from dxtbx.imageset import ImageSetLazy

                iset = ImageSetLazy(
                    ImageSetData(
                        reader=reader,
                        masker=None,
                        vendor=vendor,
                        params=params,
                        format=cls,
                    ),
                    indices=single_file_indices,
                )
                return iset
            # Create the imageset
            from dxtbx.imageset import ImageSet

            iset = ImageSet(
                ImageSetData(
                    reader=reader, masker=None, vendor=vendor, params=params, format=cls
                ),
                indices=single_file_indices,
            )

            # If any are None then read from format
            if [beam, detector, goniometer, scan].count(None) != 0:

                # Get list of models
                beam = []
                detector = []
                goniometer = []
                scan = []
                for i in range(format_instance.get_num_images()):
                    beam.append(format_instance.get_beam(i))
                    detector.append(format_instance.get_detector(i))
                    goniometer.append(format_instance.get_goniometer(i))
                    scan.append(format_instance.get_scan(i))

            if single_file_indices is None:
                single_file_indices = list(range(format_instance.get_num_images()))

            # Set the list of models
            for i in range(len(single_file_indices)):
                iset.set_beam(beam[single_file_indices[i]], i)
                iset.set_detector(detector[single_file_indices[i]], i)
                iset.set_goniometer(goniometer[single_file_indices[i]], i)
                iset.set_scan(scan[single_file_indices[i]], i)

        else:

            # Get the template
            template = filenames[0]

            # Check indices are sequential
            if single_file_indices is not None:
                assert all(
                    i + 1 == j
                    for i, j in zip(single_file_indices[:-1], single_file_indices[1:])
                )
                num_images = len(single_file_indices)
            else:
                num_images = format_instance.get_num_images()

            # Check the scan makes sense - we must want to use <= total images
            if scan is not None:
                assert scan.get_num_images() <= num_images

            # If any are None then read from format
            if beam is None:
                beam = format_instance.get_beam()
            if detector is None:
                detector = format_instance.get_detector()
            if goniometer is None:
                goniometer = format_instance.get_goniometer()
            if scan is None:
                scan = format_instance.get_scan()
                if scan is not None:
                    for f in filenames[1:]:
                        format_instance = cls(f, **format_kwargs)
                        scan += format_instance.get_scan()

            # Create the masker
            if format_instance is not None:
                masker = format_instance.get_masker(goniometer=goniometer)
            else:
                masker = None

            isetdata = ImageSetData(
                reader=reader,
                masker=masker,
                vendor=vendor,
                params=params,
                format=cls,
                template=template,
            )

            # Create the sweep
            iset = ImageSweep(
                isetdata,
                beam=beam,
                detector=detector,
                goniometer=goniometer,
                scan=scan,
                indices=single_file_indices,
            )

        # Return the imageset
        return iset