import vitest from '@vitest/eslint-plugin';
import tseslint from 'typescript-eslint';

/*
 * Catching a footgun where expects() calls without a chained matcher 
 * do nothing and pass vacuously.
 * In particular, if you forget the "ToThrow" on the following : `expect(() => bindingsArrayHelper.setArrayBasicTypeByValue(new Array(5))).toThrow();`
 * May extend to more rules later.
 */
export default [
  {
    files: ['src/**/*.test.ts'],
    languageOptions: {
      parser: tseslint.parser,
    },
    plugins: {
      vitest,
    },
    rules: {
      'vitest/valid-expect': 'error',
    },
  },
];
