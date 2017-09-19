__global__ void Core(float *R, int k, int m) {
	int tid = threadIdx.x + blockIdx.x * blockDim.x;
	while (tid < (m - k - 1)*(m - k - 1)) {
		int j = tid / (m - k - 1);
		int l = tid % (m - k - 1);
		if (j <= l) {
			j += (k + 1);
			l += (k + 1);
			float Rkj = R[k * m + j];
			float Rkk = R[k * m + k];
			R[j*m + l] = R[j*m + l] - R[k*m + l] * Rkj / (Rkk);
		}
		tid += blockDim.x * gridDim.x;
	}
}

__global__ void Core2(float *R, int k, int m) {
	int tid = k + threadIdx.x + blockIdx.x * blockDim.x;
	while (tid < m) {
		float Rkk = R[k * m + k];
		R[k*m + tid] = R[k*m + tid] / sqrt(Rkk);

		tid += blockDim.x * gridDim.x;
	}
}

void CholeskyAlgorithm(float *R, int m) {
	for (int k = 0; k < m - 1; k++) {
		Core << <blocksPerGrid, threadsPerBlock >> >(R, k, m);
		Core2 << <blocksPerGrid, threadsPerBlock >> >(R, k, m);
	}

}