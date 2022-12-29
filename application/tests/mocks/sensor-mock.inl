namespace ApplicationTests
{
	Middleware::ReturnCode SensorMock::GetMeasureData(double* out_value)
	{
		*out_value = measured_value_;
		return Middleware::ReturnCode::OK;
	}
}