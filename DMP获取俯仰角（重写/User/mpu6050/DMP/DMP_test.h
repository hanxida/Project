#ifndef __DMP_TEST_H
#define __DMP_TEST_H


unsigned char run_self_test(void);
unsigned short inv_orientation_matrix_to_scalar(
    const signed char *mtx);
unsigned short inv_row_2_scale(const signed char *row);
unsigned char mpu_dmp_init(void);
unsigned char mpu_dmp_get_data(float *pitch,float *roll,float *yaw);
void DMP_test(void);
#endif
