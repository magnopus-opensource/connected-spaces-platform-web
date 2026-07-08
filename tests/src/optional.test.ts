import { describe, it, expect, beforeAll } from 'vitest';
import { loadCSP } from '../loadModule';
import type { MainModule } from 'connected-spaces-platform-bindings';

describe('Optional bindings', () => {
  let csp: MainModule;

  beforeAll(async () => {
    csp = await loadCSP();
  });

  // Basic type tests

  it('Optional round trip basic type by value', () => {
    using helper = csp.ContainerBindingMechanismsTestType.create();
    const value = 123;

    helper.setOptionalBasicTypeByValue(value);
    const roundTrip = helper.getOptionalBasicTypeByValue();

    expect(roundTrip).toBe(value);
  });

  it('Optional round trip basic type undefined by value', () => {
    using helper = csp.ContainerBindingMechanismsTestType.create();

    helper.setOptionalBasicTypeByValue(undefined);
    const roundTrip = helper.getOptionalBasicTypeByValue();

    expect(roundTrip).toBeUndefined();
  });

  it('Optional basic type equality', () => {
    using helper = csp.ContainerBindingMechanismsTestType.create();
    const value = 123;

    helper.setOptionalBasicTypeByValue(value);
    const roundTrip = helper.getOptionalBasicTypeByValue();

    expect(csp.optionalEquals(roundTrip, value)).toBe(true);
  });

  it('Optional basic type undefined equality', () => {
    using helper = csp.ContainerBindingMechanismsTestType.create();
    const value = undefined;

    helper.setOptionalBasicTypeByValue(value);
    const roundTrip = helper.getOptionalBasicTypeByValue();

    expect(csp.optionalEquals(roundTrip, value)).toBe(true);
  });

  it('Optional round trip basic type by const ref', () => {
    using helper = csp.ContainerBindingMechanismsTestType.create();
    const value = 123;

    helper.setOptionalBasicTypeByConstRef(value);
    const roundTrip = helper.getOptionalBasicTypeByConstRef();

    expect(roundTrip).toBe(value);
  });

  it('Optional round trip basic type undefined by const ref', () => {
    using helper = csp.ContainerBindingMechanismsTestType.create();

    helper.setOptionalBasicTypeByConstRef(undefined);
    const roundTrip = helper.getOptionalBasicTypeByConstRef();

    expect(roundTrip).toBeUndefined();
  });

  // Full type tests

  it('Optional round trip full type by value', () => {
    using helper = csp.ContainerBindingMechanismsTestType.create();
    using elem = csp.BindingsTestType.create(1, 'one');

    helper.setOptionalFullTypeByValue(elem);
    const roundTrip = helper.getOptionalFullTypeByValue();

    expect(csp.optionalEquals(roundTrip, elem)).toBe(true);
  });

  it('Optional round trip full type undefined by value', () => {
    using helper = csp.ContainerBindingMechanismsTestType.create();

    helper.setOptionalFullTypeByValue(undefined);
    const roundTrip = helper.getOptionalFullTypeByValue();

    expect(roundTrip).toBeUndefined();
  });

  it('Optional round trip full type by const ref', () => {
    using helper = csp.ContainerBindingMechanismsTestType.create();
    using elem = csp.BindingsTestType.create(1, 'one');

    helper.setOptionalFullTypeByConstRef(elem);
    const roundTrip = helper.getOptionalFullTypeByConstRef();

    expect(csp.optionalEquals(roundTrip, elem)).toBe(true);
  });

  it('Optional round trip full type undefined by const ref', () => {
    using helper = csp.ContainerBindingMechanismsTestType.create();

    helper.setOptionalFullTypeByConstRef(undefined);
    const roundTrip = helper.getOptionalFullTypeByConstRef();

    expect(roundTrip).toBeUndefined();
  });

  // String tests

  it('Optional round trip string by value', () => {
    using helper = csp.ContainerBindingMechanismsTestType.create();
    const value = 'hello';

    helper.setOptionalStringByValue(value);
    const roundTrip = helper.getOptionalStringByValue();

    expect(csp.optionalEquals(roundTrip, value)).toBe(true);
  });

  it('Optional round trip string by const ref', () => {
    using helper = csp.ContainerBindingMechanismsTestType.create();
    const value = 'hello';

    helper.setOptionalStringByConstRef(value);
    const roundTrip = helper.getOptionalStringByConstRef();

    expect(csp.optionalEquals(roundTrip, value)).toBe(true);
  });
});
