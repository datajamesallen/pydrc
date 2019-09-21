"""
Dose Response Curve fitting package for python
"""
from setuptools import find_packages, setup
from distutils.core import Extension
import platform

dependencies = []

operating_system = platform.system()

print(platform.system)
libdir = ''

if platform.system == 'Linux':
    libdir = 'vartools/oocytes/lib/linux'
if platform.system == 'Darwin':
    libdir = 'vartools/oocytes/lib/macos'

module1 = Extension('_oofit',
                    include_dirs = ['include'],
                    libraries = ['levmar','lapack','blas','m'],
                    library_dirs = ['lib/linux'],
                    extra_compile_args = ["-fPIC"],
                    sources = ['pydrc/oofit.c', 'pydrc/oofit_wrap.c'])

setup(
    name='pydrc',
    version='0.0.1',
    url='https://github.com/datajamesallen/pydrc',
    license='MIT',
    author='James Allen',
    author_email='jpatallen@protonmail.com',
    description='Dose Response Curve fitting package for python',
    long_description=__doc__,
    packages=find_packages(exclude=['tests']),
    include_package_data=True,
    zip_safe=False,
    platforms='any',
    install_requires=dependencies,
    ext_modules = [module1],
    classifiers=[
        # As from http://pypi.python.org/pypi?%3Aaction=list_classifiers
        # 'Development Status :: 1 - Planning',
        'Development Status :: 2 - Pre-Alpha',
        # 'Development Status :: 3 - Alpha',
        # 'Development Status :: 4 - Beta',
        # 'Development Status :: 5 - Production/Stable',
        # 'Development Status :: 6 - Mature',
        # 'Development Status :: 7 - Inactive',
        'Environment :: Console',
        'Intended Audience :: Developers',
        'License :: OSI Approved :: BSD License',
        'Operating System :: POSIX',
        'Operating System :: MacOS',
        'Operating System :: Unix',
        'Operating System :: Microsoft :: Windows',
        'Programming Language :: Python',
        # 'Programming Language :: Python :: 2',
        'Programming Language :: Python :: 3',
        'Topic :: Software Development :: Libraries :: Python Modules',
    ]
)
