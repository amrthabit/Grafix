#pragma once

//sun
float get_sun_rotate_angle_around_itself(float day) {
	return day * 360.0f / 27.0f;
}
//earth
float get_earth_rotate_angle_around_sun(float day) {

	return day * 360.0f / 365.0f;
}
float get_earth_rotate_angle_around_itself(float day)
{
	return day * 360.0f / 1.0f;
}
//moon
float get_moon_rotate_angle_around_earth(float day)
{
	return day * 360.0f / 27.0f;
}
float get_moon_rotate_angle_around_itself(float day)
{
	return day * 360.0f / 27.0f;
}