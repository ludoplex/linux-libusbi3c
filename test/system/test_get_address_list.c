/***************************************************************************
  USBI3C  -  Library to talk to I3C devices via USB.
  -------------------
  copyright            : (C) 2022 Intel Corporation
  SPDX-License-Identifier: LGPL-2.1-only
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU Lesser General Public License           *
 *   version 2.1 as published by the Free Software Foundation.             *
 *                                                                         *
 ***************************************************************************/

#include "helpers/test_helpers.h"

struct test_deps {
	struct usbi3c_device *usbi3c_dev;
};

static int test_setup(void **state)
{
	struct test_deps *deps = (struct test_deps *)malloc(sizeof(struct test_deps));

	deps->usbi3c_dev = helper_usbi3c_controller_init();
	assert_int_equal(usbi3c_get_usb_error(deps->usbi3c_dev), RETURN_SUCCESS);

	*state = deps;

	return 0;
}

static int test_teardown(void **state)
{
	struct test_deps *deps = (struct test_deps *)*state;

	usbi3c_device_deinit(&deps->usbi3c_dev);
	free(deps);

	return 0;
}

/* Test to verify the user can get the list of addresses from devices in the bus */
static void test_usbi3c_get_address_list(void **state)
{
	struct test_deps *deps = (struct test_deps *)*state;
	uint8_t *addresses = NULL;
	int devices_in_bus = -1;
	const int DEVICE_ADDRESS = 5;

	devices_in_bus = usbi3c_get_address_list(deps->usbi3c_dev, &addresses);
	assert_int_equal(devices_in_bus, 1);
	assert_int_equal(usbi3c_get_usb_error(deps->usbi3c_dev), RETURN_SUCCESS);
	assert_int_equal(addresses[0], DEVICE_ADDRESS);

	free(addresses);
}

int main(void)
{
	/* Unit tests for the usbi3c_get_address_list() function */
	const struct CMUnitTest tests[] = {
		cmocka_unit_test_setup_teardown(test_usbi3c_get_address_list, test_setup, test_teardown),
	};

	return cmocka_run_group_tests(tests, NULL, NULL);
}
