const {
    fromZigbeeConverters,
    exposes
} = require('zigbee-herdsman-converters');
const {
    postfixWithEndpointName
} = require('zigbee-herdsman-converters/lib/utils');
const bind = async (endpoint, target, clusters) => {
    for (const cluster of clusters) {
        await endpoint.bind(cluster, target);
    }
};
const CHANNELS_STARTING_ENDPOINT = 2;
const CHANNELS_COUNT = 3;
const ACCESS_STATE = 0b001,
    ACCESS_WRITE = 0b010,
    ACCESS_READ = 0b100;

const defaultReportingConfig = {
    minimumReportInterval: 60,
    maximumReportInterval: 60 * 30,
    reportableChange: 1,
};



function getKey(object, value, fallback, convertTo) {
    for (const key in object) {
        if (object[key] === value) {
            return convertTo ? convertTo(key) : key;
        }
    }

    return fallback;
}


const endpointNames = {
    l1: 2,
    l2: 3,
    l3: 4
};

const device = {
    zigbeeModel: ['DIYRuZ_WaterMon'],
    model: 'DIYRuZ_WaterMon',
    vendor: 'DIYRuZ',
    description: '[DIYRuZ_WaterMon](http://xxx.ru)',
    supports: '',
    fromZigbee: [{
            cluster: 'genAnalogInput',
            type: ['attributeReport', 'readResponse'],
            convert: (model, msg, publish, options, meta) => {
                const adcPropName = postfixWithEndpointName('adc_value', msg, model);
                const {
                    presentValue
                } = msg.data;
                return {
                    [adcPropName]: presentValue.toFixed(3)
                }
            },
        },
        {
            cluster: 'genMultistateInput',
            type: ['attributeReport', 'readResponse'],
            convert: (model, msg, publish, options, meta) => {
                const totalPropName = postfixWithEndpointName('total_value', msg, model);
                const presentPropName = postfixWithEndpointName('present_value', msg, model);
                const deltaPropName = postfixWithEndpointName('delta_value', msg, model);
                const {
                    state = {}
                } = meta;
                const totalValue = state[totalPropName] || 0;
                const prevValue = state[presentPropName] || 0;

                const {
                    presentValue
                } = msg.data;

                const diff = prevValue <= presentValue ? presentValue - prevValue : presentValue;

                return {
                    [presentPropName]: presentValue,
                    [totalPropName]: totalValue + diff,
                    [deltaPropName]: diff
                }

            }
        },
        fromZigbeeConverters.device_temperature,
        fromZigbeeConverters.battery
    ],
    toZigbee: [{
        key: ['action'],

        convertSet: async (entity, key, value, meta) => {
            const result = {};
            switch (value) {
                case 'clear_stats':
                    await entity.write('genMultistateInput', {
                        presentValue: 0
                    });
                    const names = ['total_value', 'present_value', 'delta_value']
                    const endpointName = getKey(endpointNames, entity.ID);
                    result.state = {};
                    names.forEach(name => {
                        result.state[`${name}_${endpointName}`] = 0;
                    });
                    break;
                default:
                    meta.logger.error('Not implemented' + value);
                    break;
            }
            return result;

        },
    }],
    meta: {
        configureKey: 11,
        multiEndpoint: true,
        battery: {
            voltageToPercentage: '3V_2500_3200' // FYI: https://github.com/Koenkk/zigbee-herdsman-converters/blob/239822497aae9ab51d22c80206b9a42a4265a94a/lib/utils.js#L118
        }
    },
    configure: async (device, coordinatorEndpoint) => {
        for (let i = 0; i < CHANNELS_COUNT; i++) {
            const firstEp = device.getEndpoint(1);

            await bind(firstEp, coordinatorEndpoint, [
                'genDeviceTempCfg', 'genPowerCfg'
            ]);
            await firstEp.configureReporting('genDeviceTempCfg', [{
                ...defaultReportingConfig,
                attribute: 'currentTemperature'
            }]);
            await firstEp.configureReporting('genPowerCfg', [{
                ...defaultReportingConfig,
                attribute: 'batteryVoltage',
            }]);


            const ep = device.getEndpoint(CHANNELS_STARTING_ENDPOINT + i);
            await bind(ep, coordinatorEndpoint, [
                'genMultistateInput', 'genAnalogInput'
            ]);
            await ep.configureReporting('genMultistateInput', [{
                ...defaultReportingConfig,
                attribute: 'presentValue',
            }]);
            await ep.configureReporting('genAnalogInput', [{
                ...defaultReportingConfig,
                attribute: 'presentValue',
            }]);
        }
    },
    exposes: [
        exposes.presets.device_temperature(),
        exposes.numeric('voltage', ACCESS_STATE).withDescription('Battery voltage').withUnit('mV'),
        exposes.presets.battery(),

        exposes.numeric('present_value', ACCESS_STATE).withEndpoint('l1').withDescription('Current data in device memory'),
        exposes.numeric('present_value', ACCESS_STATE).withEndpoint('l2').withDescription('Current data in device memory'),
        exposes.numeric('present_value', ACCESS_STATE).withEndpoint('l3').withDescription('Current data in device memory'),

        exposes.numeric('delta_value', ACCESS_STATE).withEndpoint('l1').withDescription('Last reported value'),
        exposes.numeric('delta_value', ACCESS_STATE).withEndpoint('l2').withDescription('Last reported value'),
        exposes.numeric('delta_value', ACCESS_STATE).withEndpoint('l3').withDescription('Last reported value'),


        exposes.numeric('total_value', ACCESS_STATE).withEndpoint('l1').withDescription('Total value processed by device'),
        exposes.numeric('total_value', ACCESS_STATE).withEndpoint('l2').withDescription('Total value processed by device'),
        exposes.numeric('total_value', ACCESS_STATE).withEndpoint('l3').withDescription('Total value processed by device'),


        exposes.enum('action', ACCESS_WRITE, ['clear_stats']).withEndpoint('l1').withDescription("Actions"),
        exposes.enum('action', ACCESS_WRITE, ['clear_stats']).withEndpoint('l2').withDescription("Actions"),
        exposes.enum('action', ACCESS_WRITE, ['clear_stats']).withEndpoint('l3').withDescription("Actions"),


        exposes.numeric('adc_value', ACCESS_STATE).withEndpoint('l1').withDescription('ADC voltage').withUnit('V'),
        exposes.numeric('adc_value', ACCESS_STATE).withEndpoint('l2').withDescription('ADC voltage').withUnit('V'),


    ],
    endpoint: (device) => endpointNames
};

module.exports = device;