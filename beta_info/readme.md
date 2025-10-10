# Info

## get_par_kinematics

### how to run

`root get_par_kinematics.cpp`

### info

+ input file: `kinematics.txt`
+ output file: `par_kinematics_p.txt`, `par_kinematics_r.txt`
+ The dynamic curve of the 32S + 150Sm reaction at 102 MeV was calculated using LISE++. The target thickness was set to 1.2 mg/cm², and the reaction was assumed to occur at the center of the target during the simulation.
+ The input file used for the simulation was `kinematics.txt`. The program `get_par_kinematics.cpp` was used to extract the dynamic curve parameters. The resulting kinematic function parameters were recorded separately in two output files:  `par_kinematics_p.txt` for projectile-like fragments, `par_kinematics_r.txt` for recoil-like fragments

## get_par_correlations

### how to run

`root get_par_correlations.cpp`

### info

+ input file: `correlation.txt`
+ output file: `par_correlation.txt`
+ The correction curve of the 32S + 150Sm reaction at 102 MeV was also calculated using LISE++.
+ The input file used for the simulation was `correlation.txt`. The program `get_par_correlation.cpp` was used to extract the correlation curve parameters. The resulting correlation function parameters were recorded in `par_correlation.txt` output files

## `e_out` for projectile

### how to run

`root get_theta_eloss_p.cpp`

### info

- input file: `theta_spider.txt`, `theta_s3.txt`, `par_kinematics_p.txt`
- output file: `theta_ein_eout_p.txt`
- The scattering angles of the projectile nucleus 32S are obtained by reading the files `theta_spider.txt` and `theta_s3.txt`.
- Based on the scattering angles and the kinematic function parameters stored in the file `par_kinematics_p.txt`, the energy of 32S at the target center (i.e., immediately after the reaction) is calculated.
- Given the energy and angle at the target center, the program `elast` is then used to compute the energy of 32S after exiting the target material, taking into account energy loss.
  This process is implemented in the script `get_theta_eloss_p.cpp`. The output is written to the file `theta_ein_eout_p.txt`, which contains three columns:
- `theta` — the scattering angle (in degrees)
- `e_in` — the energy at the target center (in MeV)
- `e_out` — the energy after exiting the target (in MeV)

## `e_out` for recoil

### how to run

`root get_theta_eloss_r.cpp`

### info

- input file: `theta_spider.txt`, `theta_s3.txt`, `par_kinematics_r.txt`
- output file: `theta_ein_eout_r.txt`
- The scattering angles of the projectile nucleus 32S are obtained by reading the files `theta_spider.txt` and `theta_s3.txt`.
- Using these angles and the kinematic function parameters stored in `par_kinematics_p.txt`, the energy of 32S at the target center (i.e., immediately after the reaction) is calculated.
- With this information, and applying the correction parameters in `par_correction.txt`, the corresponding scattering angle and energy of the recoil nucleus 150Sm can be determined.
- Given the energy and angle of 150Sm at the target center, the `elast` program is used to calculate its energy after exiting the target, accounting for energy loss.
- This entire process is implemented in the script `get_theta_eloss_r.cpp`. The output is written to the file `theta_ein_eout_r.txt`, which contains the following four columns:
- `theta_p` — scattering angle of 32S (in degrees)
- `theta_r` — scattering angle of 150Sm (in degrees)
- `e_in` — energy of 150Sm at the target center (in MeV)
- `e_out` — energy of 150Sm after exiting the target (in MeV)

## draw_theta_eloss.cpp

### how to run

```
> root
> .L draw_theta_eloss.cpp
> draw_theta_eloss_elast()
> draw_theta_eloss_lise()
```

### info

+ input files: `par_kinematics_p.txt`, `par_kinematics_r.txt`, `par_correlation.txt`, `kinematics_after_target.txt`
+ output files: some figures.
+ use lise++, choose kinematic after target.
+ unction `draw_theta_eloss_lise()` in `draw_theta_eloss.cpp` can be used to draw the figure.



