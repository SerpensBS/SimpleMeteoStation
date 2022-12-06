namespace ApplicationTests
{
	Middleware::ReturnCode SensorMock::GetData(double* out_value)
	{
		*out_value = measured_value;
		return Middleware::ReturnCode::OK;
	}
}